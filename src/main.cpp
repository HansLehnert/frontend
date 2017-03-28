#include <iostream>
#include <vector>
#include <string>

#include <dirent.h>
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
	context.init();

	//Create folders
<<<<<<< HEAD
	if (createDir("game"))
		std::cout << "[System]\tCreated 'game' directory" << std::endl;
	if (createDir("emulator"))
		std::cout << "[System]\tCreated 'emulator' directory" << std::endl;
	if (createDir("logo"))
		std::cout << "[System]\tCreated 'logo' directory" << std::endl;
	if (createDir("dat"))
		std::cout << "[System]\tCreated 'dat' directory" << std::endl;

=======
	//createDir("game");
	//createDir("emulator");
	//createDir("logo");
	//createDir("dat");
>>>>>>> 93ed981e3f462a2cd8d576b64e72c650451107f1

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
