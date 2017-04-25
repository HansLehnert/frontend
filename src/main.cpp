#include <iostream>
#include <vector>
#include <string>

#include <dirent.h>
#include <unistd.h>

#include "Context.h"
#include "Config.h"
#include "GameManager.h"
#include "objects/Background.h"
#include "objects/Marquee.h"

//Creates directory
//(Currently only works on UNIX system)
bool createDir(std::string dir_name) {
	DIR* dir = opendir(dir_name.c_str());

	if (dir == NULL) {
		closedir(dir);

		std::string command = "mkdir " + dir_name;

		if (system(command.c_str()) == 0)
			return true;
		else
			return false;
	}
	else {
		closedir(dir);
		return false;
	}
}


int main() {
	Context context;
	if (!context.init()) {
		return 0;
	}

	Text::initFreetype();
	Text::loadTypeface("Oswald-Regular.ttf");

	//Create folders
	if (createDir("game"))
		std::cout << "[System]\tCreated 'game' directory" << std::endl;
	if (createDir("emulator"))
		std::cout << "[System]\tCreated 'emulator' directory" << std::endl;
	if (createDir("logo"))
		std::cout << "[System]\tCreated 'logo' directory" << std::endl;
	if (createDir("dat"))
		std::cout << "[System]\tCreated 'dat' directory" << std::endl;


	Config main_config("settings");
	main_config.setValue("rom_path", main_config["rom_path"]);
	main_config.setValue("emulator_path", main_config["emulator_path"]);


	GameManager game_manager(&context, &main_config);

	Image title_image;
	Marquee marquee;
	Background background(6);

	//Game selection (temporal)
	int selection = 0;
	title_image.setContent((game_manager.getGameList()->begin()->second)["game_logo"]);
	marquee.updateImage((game_manager.getGameList()->begin()->second)["screenshot"]);
	marquee.addListener(EVENT_INPUT_KEYDOWN, [&selection, &game_manager, &marquee, &title_image] (Event event) {
		if (event.input.key == KEY_DOWN) {
			selection++;
		}
		else if (event.input.key == KEY_UP) {
			selection--;
		}

		if (selection < 0) {
			selection = 0;
		}
		if (selection >= game_manager.getGameList()->size()) {
			selection = game_manager.getGameList()->size() - 1;
		}

		auto game = game_manager.getGameList()->begin();
		for (int i = 0; i < selection; i++)
			game++;

		marquee.updateImage((game->second)["screenshot"]);
		if (title_image.setContent((game->second)["game_logo"])) {
			title_image.visible = true;
		}
		else {
			title_image.visible = false;
		}

		if (event.input.key == KEY_SELECT) {
			game_manager.launchGame(game->first);
		}
	});

	//Set the world matrix for vertical orientation
	Object::world_matrix[0][0] = 0;
	Object::world_matrix[0][1] = 1;
	Object::world_matrix[1][0] = -1 / context.getAspectRatio();
	Object::world_matrix[1][1] = 0;


	float marquee_position = context.getAspectRatio() - 0.5;

	marquee.setPosition(glm::vec3(0, marquee_position, 0));
	title_image.setPosition(glm::vec3(0, marquee_position, 0));


	//Main loop
	while (context.poll()) {
		context.swapBuffers();
		Object::updateAll();
		Object::renderAll();
	}

	return 1;
}
