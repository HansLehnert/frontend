#include "Scrapper.h"

#include <map> 
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <curl/curl.h>

#include "pugi/pugixml.hpp"
#include "Config.h"


bool file_exists(std::string filename) {
	std::ifstream file(filename);
	bool result = file.good();
	file.close();

	return result;
}



//Callback for curl
size_t write_callback(char* data, size_t size, size_t nmemb, void* dest) {
	std::string* dest_string = (std::string*)dest;
	int dest_size = dest_string->size();

	dest_string->append(data, nmemb);

	return dest_string->size() - dest_size;
}


void scrapDatFiles(std::map<std::string, Config>* game_list, std::map<std::string, Config>* emulator_list) {
	//std::map<std::string, char*> dat_src;
	std::map<std::string, pugi::xml_document> dat_xml;

	std::cout << "[Scrapper]\tLoading .dat files" << std::endl;
	for (auto& i : *emulator_list) {
		bool dat_exists;
		std::string dat_path = i.second.getValue("dat_file", &dat_exists);

		if (dat_exists) {
			std::cout << "[Scrapper]\tFor emulator "
			          << i.first
			          << " reading "
			          << dat_path
			          << std::endl;

			std::ifstream dat_file(dat_path);

			if (!dat_file.is_open()) {
				std::cout << "\t\tFailed to open" << std::endl;
				continue;
			}

			//Read entire dat file into buffer
			char* dat_buffer;
			int file_size;

			dat_file.seekg(0, dat_file.end);
			file_size = dat_file.tellg();
			dat_buffer = new char[file_size + 1];

			dat_file.seekg(0, dat_file.beg);
			dat_file.get(dat_buffer, file_size + 1, '\0');
			dat_file.close();

			//Parse XML
			//pugi::xml_parse_result result = dat_xml[i.first].load_buffer(dat_buffer, file_size);
			dat_xml[i.first].load_buffer(dat_buffer, file_size);
			delete dat_buffer;
		}
	}

	for (auto &i : *game_list) {
		Config* game = &(i.second);

		std::string game_name = (*game)["game_name"];

		std::cout << "[Scrapper]\tLooking up " << i.first << std::endl;

		//Find game name from .dat file
		if (game_name == "") {
			std::cout << "\t\tMissing name" << std::endl;

			std::string emulator = (*game)["emulator"];
			if (emulator == "")
				std::cout << "\t\tMissing emulator" << std::endl;

			//Check all dat files
			for (auto& dat : dat_xml) {
				//If game has emulator only check that dat file
				if (emulator != "" && emulator != dat.first)
					continue;

				for (pugi::xml_node& node : dat.second.child("datafile").children("game")) {
					if (node.attribute("name").value() == i.first) {
						game_name = node.child("description").child_value();
						game->setValue("game_name", game_name);
						std::cout << "\t\tNew name: " << game_name << std::endl;

						if (emulator == "") {
							emulator = dat.first;
							game->setValue("emulator", emulator);
							std::cout << "\t\tNew emulator: " << emulator << std::endl;
						}
						break;
					}
				}
			}
		}
	}
}


void scrapTheGamesDB(std::map<std::string, Config>* game_list, std::map<std::string, Config>* emulator_list) {
	std::cout << "[Scrapper]\tUsing thegamesdb.net" << std::endl;

	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);

	for (auto &i : *game_list) {
		Config* game = &(i.second);

		std::string game_name = (*game)["game_name"];
		std::string game_logo = (*game)["game_logo"];
		std::string game_art = (*game)["game_art"];

		//If game doesn't have a name, dont look for info
		if (game_name == "")
			continue;
		
		std::cout << "[Scrapper]\tLooking up " << i.first << std::endl;
		
		std::string clear_name = cleanName(game_name);
		
		if (game_logo == "") {
			std::cout << "\t\tMissing logo" << std::endl;

			std::string api_response;
			std::string api_request = "http://www.thegamesdb.net/api/GetGame.php?name=";

			char* encoded_name = curl_easy_escape(curl_handle, clear_name.c_str(), 0);
			api_request += encoded_name;
			curl_free(encoded_name);

			if (!getFile(api_request, &api_response, curl_handle)) {
				std::cout << "\t\tFailed to get API response" << std::endl;
				continue;
			}

			//Parse data
			pugi::xml_document info_xml;
			info_xml.load_buffer(api_response.c_str(), api_response.size());

			std::string base_img_url = info_xml.child("Data").child("baseImgUrl").child_value();
			std::string logo_url;

			std::cout << "Clean name:\t" << clear_name << std::endl;

			//Search logo
			bool found = false;
			for (pugi::xml_node& node : info_xml.child("Data").children("Game")) {
				std::string entry_name = cleanName(node.child("GameTitle").child_value());
				int platform_id = node.child("PlatformId").text().as_int();

				std::cout << "Entry name:\t" << entry_name << std::endl;

				if (entry_name == clear_name) {
					if (node.child("Images").child("clearlogo")) {
						logo_url = node.child("Images").child("clearlogo").child_value();
						found = true;

						if (platform_id == 23)
							break;
					}
				}

				//check alt names
				for (pugi::xml_node& alt_name : node.child("AlternateTitles").children("title")) {
					entry_name = cleanName(alt_name.child_value());

					std::cout << "\t\t" << entry_name << std::endl;

					if (entry_name == clear_name) {
						if (node.child("Images").child("clearlogo")) {
							logo_url = node.child("Images").child("clearlogo").child_value();
							found = true;

							if (platform_id == 23)
								break;
						}
					}
				}
			}

			//Save logo
			if (found) {
				std::string logo_filename = "logo/" + i.first + ".png";
				if (saveFile(base_img_url + logo_url, logo_filename, curl_handle)) {
					game->setValue("game_logo", logo_filename);
					std::cout << "\t\tLogo found" << std::endl;
				}
				else {
					std::cout << "\t\tFailed to load logo" << std::endl;	
				}
			}
			else {
				std::cout << "\t\tLogo not found" << std::endl;
			}
		}

		game->write();
	}

	curl_global_cleanup();
}


void scrapArcadeItalia(std::map<std::string, Config>* game_list, std::map<std::string, Config>* emulator_list) {
	std::cout << "[Scrapper]\tUsing adb.arcadeitalia.net" << std::endl;

	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);

	for (auto &i : *game_list) {
		std::string rom_name = i.first;
		Config* game = &(i.second);
		
		std::cout << "[Scrapper]\tLooking up " << rom_name << std::endl;
		
		//Download game logo
		std::string logo_filename = "logo/" + i.first + ".png";
		if (!file_exists((*game)["game_logo"])) {
			if (file_exists(logo_filename)) {
				//File exists but is not in the config file
				game->setValue("game_logo", logo_filename);
			}
			else {
				std::cout << "\t\tMissing logo" << std::endl;

				std::string logo_url = "http://adb.arcadeitalia.net/media/mame.current/decals/" + rom_name + ".png";

				if (saveFile(logo_url, logo_filename, curl_handle)) {
					game->setValue("game_logo", logo_filename);
					std::cout << "\t\tLogo found" << std::endl;
				}
				else {
					std::cout << "\t\tFailed to load logo" << std::endl;	
				}
			}
		}


		//Download game screenshot
		std::string screenshot_filename = "screenshot/" + i.first + ".png";
		if (!file_exists((*game)["screenshot"])) {
			if (file_exists(screenshot_filename)) {
				//File exists but is not in the config file
				game->setValue("screenshot", screenshot_filename);
			}
			else {
				std::cout << "\t\tMissing screenshot" << std::endl;

				std::string screenshot_url = "http://adb.arcadeitalia.net/media/mame.current/ingames/" + rom_name + ".png";

				if (saveFile(screenshot_url, screenshot_filename, curl_handle)) {
					game->setValue("screenshot", screenshot_filename);
					std::cout << "\t\tScreenshot found" << std::endl;
				}
				else {
					std::cout << "\t\tFailed to load screenshot" << std::endl;	
				}
			}
		}

		game->write();
	}

	curl_global_cleanup();
}


bool getFile(std::string url, std::string* dest, CURL* handle) {
	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)dest);

	int curl_status;
	long http_status;

	curl_status = curl_easy_perform(handle);
	curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_status);
	if (curl_status == CURLE_OK && http_status == 200)
		return true;
	else
		return false;
}


bool saveFile(std::string url, std::string filename, CURL* handle) {
	std::string data;
	if (getFile(url, &data, handle)) {
		std::ofstream file(filename);
		if (file.is_open()) {
			file.write(data.c_str(), data.size());
			file.close();
			return true;
		}

		return false;
	}

	return false;
}


std::string cleanName(std::string name) {
	std::string clean_name;

	//Remove punctuation, version
	for (unsigned int i = 0; i < name.length(); i++) {
		if (isalnum(name[i]))
			clean_name += tolower(name[i]);
		else if (name[i] == '('/* || name[i] == '-' || name[i] == ':'*/)
			break;
		else if (!isspace(clean_name.back()))
			clean_name += " ";
	}
	while (isspace(clean_name.back())) //erase trailing whitespace
		clean_name.erase(--clean_name.end());

	return clean_name;
}
