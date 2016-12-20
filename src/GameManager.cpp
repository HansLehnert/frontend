#include "GameManager.h"

#include <iostream>
#include <string>
#include <list>
#include <map>
#include <dirent.h>
#include <ctype.h>
#include <stdlib.h>
#include <SDL2/SDL_keycode.h>

#include "Config.h"
#include "Message.h"
#include "Scrapper.h"
#include "objects/Image.h"

const char* rom_ext = ".png";

GameManager::GameManager(std::string rom_path) {
	init(rom_path);
	selection = game_list.begin();

	Message input_request;
	input_request.type = FE_INPUT;
	input_request.input.source = this;
	input_request.input.event = FE_START_LISTEN;
	sendMessageAll(input_request);

	game_title.setScale(glm::vec2(0.05, 0.08));
	game_title.setSpacing(0.2);

	Scrapper scrapper;
	scrapper.scrap(&game_list, &emulator_list);

	updateUI();
}

void GameManager::init(std::string rom_path) {
	DIR *dir;
	struct dirent *ent;

	dir = opendir(rom_path.c_str());
	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			std::string file_name = ent->d_name;
			auto ext_point = file_name.find('.');

			std::string game_name = file_name.substr(0, ext_point);
			std::string extension = file_name.substr(ext_point);

			if (extension == rom_ext) {
				Config game_config("game/" + game_name);
				int has_value;

				game_config.getValue("emulator", &has_value);
				if (has_value == -1)
					game_config.setValue("emulator", "fba");

				game_config.getValue("launch_options", &has_value);
				if (has_value == -1)
					game_config.setValue("launch_options", "");

				game_config.write();

				game_list.insert(std::pair<std::string, Config>(game_name, game_config));
			}
		}
		closedir(dir);
	}
	else {
		std::cout << "Invalid rom path";
	}

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
	cmd += emulator->second.getValue("path");
	cmd += " " + emulator->second.getValue("options");
	cmd += " " + game_config->second.getValue("options");
	cmd += " " + game;

	std::cout << cmd << std::endl;
	system(cmd.c_str());

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

		std::string game_name = selection->second.getValue("game_name");
		game_name = game_name.substr(0, game_name.find("("));
		game_title.setContent(game_name);
		game_title.setPosition(glm::vec2(game_name.length() * -0.03, 0));
		game_title.visible = 1;
	}
}

void GameManager::sendMessage(Message message) {
	if (message.type == FE_INPUT && message.input.event == FE_KEY_DOWN) {
		switch (message.input.value) {
			case SDLK_DOWN:
				if (selection != (--game_list.end()))
					selection++;
				updateUI();
				break;
			case SDLK_UP:
				if (selection != game_list.begin())
					selection--;
				updateUI();
				break;
			case SDLK_RETURN: {
				launchGame(selection->first);
				break;
			}
		}
	}
}