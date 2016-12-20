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

//Callback for curl
size_t write_callback(char* data, size_t size, size_t nmemb, void* dest) {
	std::string* dest_string = (std::string*)dest;
	int dest_size = dest_string->size();

	dest_string->append(data, nmemb);

	return dest_string->size() - dest_size;
}


void Scrapper::scrap(std::map<std::string, Config>* game_list, const std::map<std::string, Config>* emulator_list) {
	//std::map<std::string, char*> dat_src;
	std::map<std::string, pugi::xml_document> dat_xml;

	std::cout << "[Scrapper]\tUsing thegamesdb.net" << std::endl;

	curl_global_init(CURL_GLOBAL_ALL);
	CURL* curl_handle = curl_easy_init();
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_callback);

	std::cout << "[Scrapper]\tLoading .dat files" << std::endl;
	for (auto i : *emulator_list) {
		int dat_exists;
		std::string dat_path = i.second.getValue("dat_file", &dat_exists);

		if (dat_exists != -1) {
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
		}
	}
	
	for (auto &i : *game_list) {
		std::string game_name = i.second.getValue("game_name");
		std::string game_logo = i.second.getValue("game_logo");
		std::string game_art = i.second.getValue("game_art");

		std::cout << "[Scrapper]\tLooking up " << i.first << std::endl;

		//Find game name from .dat file
		if (game_name == "") {
			std::cout << "\t\tMissing name" << std::endl;

			std::string emulator = i.second.getValue("emulator");
			if (dat_xml.find(emulator) == dat_xml.end())
				continue;


			for (pugi::xml_node node : dat_xml[emulator].child("datafile").children("game")) {
				if (node.attribute("name").value() == i.first) {
					game_name = node.child("description").child_value();
					i.second.setValue("game_name", game_name);
					std::cout << "\t\tNew name: " << game_name << std::endl;
					break;
				}
			}
		}
		
		//If game doesn't have a name, dont look for info
		if (game_name == "")
			continue;
		
		
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

			int found = 0;
			for (pugi::xml_node node : info_xml.child("Data").children("Game")) {
				std::string entry_name = cleanName(node.child("GameTitle").child_value());
				int platform_id = node.child("PlatformId").text().as_int();

				std::cout << "Entry name:\t" << entry_name << std::endl;

				if (entry_name == clear_name) {
					if (node.child("Images").child("clearlogo")) {
						logo_url = node.child("Images").child("clearlogo").child_value();
						found = 1;

						if (platform_id == 23)
							break;
					}
				}

				//check alt names
				for (pugi::xml_node alt_name : node.child("AlternateTitles").children("title")) {
					entry_name = cleanName(alt_name.child_value());

					std::cout << "\t\t" << entry_name << std::endl;

					if (entry_name == clear_name) {
						if (node.child("Images").child("clearlogo")) {
							logo_url = node.child("Images").child("clearlogo").child_value();
							found = 1;

							if (platform_id == 23)
								break;
						}
					}
				}

				if (found)
					break;
			}

			if (found) {
				std::string logo_filename = "logo/" + i.first + ".png";
				if (saveFile(base_img_url + logo_url, logo_filename, curl_handle)) {
					i.second.setValue("game_logo", logo_filename);
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

		i.second.write();
	}

	curl_global_cleanup();
}


int Scrapper::getFile(std::string url, std::string* dest, CURL* handle) {
	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)dest);

	int curl_status;
	long http_status;

	curl_status = curl_easy_perform(handle);
	curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &http_status);
	if (curl_status == CURLE_OK && http_status == 200)
		return 1;
	else
		return 0;
}


int Scrapper::saveFile(std::string url, std::string filename, CURL* handle) {
	std::string data;
	if (getFile(url, &data, handle)) {
		std::ofstream file(filename);
		if (file.is_open()) {
			file.write(data.c_str(), data.size());
			file.close();
			return 1;
		}

		return 0;
	}

	return 0;
}


std::string Scrapper::cleanName(std::string name) {
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