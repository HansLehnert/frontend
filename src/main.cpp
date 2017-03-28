#include <iostream>
#include <vector>
#include <string>

#include <unistd.h>

#include "Context.h"
#include "Config.h"
#include "GameManager.h"
#include "core/Object.h"
#include "objects/Text.h"
#include "objects/ItemList.h"
#include "objects/Background.h"
#include "objects/Image.h"


//Creates directory
//(Currently only works on UNIX system)
bool createDir(std::string dir) {
	std::string command = "mkdir " + dir;

	if (system(command.c_str()) == 0)
		return true;
	else
		return false;
}


int main() {
	Context context;
	context.init();

	//Create folders
	//createDir("game");
	//createDir("emulator");
	//createDir("logo");
	//createDir("dat");

	Config main_config("settings");
	main_config["rom_path"] = main_config["rom_path"];
	main_config.write();

	GameManager game_manager(main_config["rom_path"]);

	Background background(6);

	Object::world_matrix[0][0] = 0;
	Object::world_matrix[0][1] = context.getAspectRatio();
	Object::world_matrix[1][0] = -1;
	Object::world_matrix[1][1] = 0;

	while (context.poll()) {
		context.swapBuffers();
		Object::updateAll();
		Object::renderAll();
	}

	return 0;
}
