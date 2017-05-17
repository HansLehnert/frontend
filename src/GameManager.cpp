#include "GameManager.h"

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>
#include <SDL2/SDL_keycode.h>

#include "core/Event.h"
#include "Config.h"
#include "Scrapper.h"
#include "objects/Image.h"

const char* rom_ext = ".zip";

GameManager::GameManager(Context* p_context, Config* main_config, std::string instance_name) :
	Object(instance_name),
	context(p_context),
	config(main_config) {

	init((*config)["rom_path"]);
	selection = game_list.begin();

	scrapDatFiles(&game_list, &emulator_list);
	scrapArcadeItalia(&game_list, &emulator_list);

	if (game_list.size() > 0) {
		addListener(EVENT_INPUT_KEYDOWN);
		updateUI();
	}
	else {
		game_title.visible = false;
		game_logo.visible = false;
	}
}


void GameManager::init(std::string rom_path) {
	DIR* dir;
	struct dirent* ent;

	//Find ROMs in rom folder
	dir = opendir(rom_path.c_str());
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			std::string file_name = ent->d_name;
			auto ext_point = file_name.find('.');

			//Check if file has extension
			if (ext_point == std::string::npos)
				continue;

			std::string game_name = file_name.substr(0, ext_point);
			std::string extension = file_name.substr(ext_point);

			if (extension == rom_ext) {
				Config game_config("game/" + game_name);
				bool has_value;

				game_config.getValue("emulator", &has_value);
				if (!has_value)
					game_config.setValue("emulator", "");

				game_config.getValue("launch_options", &has_value);
				if (!has_value)
					game_config.setValue("launch_options", "");

				game_config.getValue("path", &has_value);
				if (!has_value)
					game_config.setValue("path", rom_path + "/" + file_name);

				game_list.insert(std::pair<std::string, Config>(game_name, game_config));
			}
		}
		closedir(dir);
	}
	else {
		std::cout << "[GameManager]\tInvalid rom path ("
		          << rom_path << ")" << std::endl;
	}

	//Read emulator configuration
	dir = opendir("emulator");
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			std::string file_name = ent->d_name;
			if (!isalnum(file_name[0])) {
				continue;
			}

			Config emulator_config("emulator/" + file_name);

			emulator_list.insert(std::pair<std::string, Config>(file_name, emulator_config));
		}
		closedir(dir);
	}
}


int GameManager::launchGame(std::string game) {
	auto game_config = game_list.find(game);
	if (game_config == game_list.end()) {
		return -1;
	}
	
	auto emulator = emulator_list.find(game_config->second.getValue("emulator"));
	if (emulator == emulator_list.end()) {
		return -1;
	}

	std::string cmd = "";
	cmd += (*config)["emulator_path"];
	cmd += " -L " + emulator->second.getValue("path");
	cmd += " " + emulator->second.getValue("options");
	cmd += " " + game_config->second.getValue("options");
	cmd += " " + game_config->second.getValue("path");

	std::cout << "[GameManager]\tLaunching " << game << std::endl;

	context->pause();
	system(cmd.c_str());
	context->resume();

	return 0;
}


std::vector<std::string> GameManager::getGameList() {
	std::vector<std::string> result;
	for (auto game : game_list) {
		result.push_back(game.first);
	}
	return result;
}


void GameManager::updateUI() {
	if (game_logo.setContent(selection->second.getValue("game_logo"))) {
		game_logo.setSize(glm::vec2(0.6, 0.2));
		game_logo.visible = 1;

		game_title.visible = 0;
	}
	else {
		game_logo.visible = 0;


		std::string game_name = selection->second["game_name"];
		if (game_name == "")
			game_name = selection->first;
		game_name = game_name.substr(0, game_name.find("("));
		game_title.setText(game_name);
		game_title.visible = 1;
	}
}


void GameManager::handleEvent(Event& event) {
	if (event.type == EVENT_INPUT_KEYDOWN) {
		switch (event.input.key) {
			case KEY_DOWN:
				if (selection != (--game_list.end()))
					selection++;
				updateUI();
				break;

			case KEY_UP:
				if (selection != game_list.begin())
					selection--;
				updateUI();
				break;

			case KEY_SELECT:
				launchGame(selection->first);
				break;

			default:
				break;
		}
	}
}
