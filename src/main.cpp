#include <iostream>
#include <vector>

#include "Context.h"
#include "Config.h"
#include "GameManager.h"
#include "core/Object.h"
#include "objects/Text.h"
#include "objects/ItemList.h"
#include "objects/Background.h"
#include "objects/Image.h"

int main(void) {
	Context context;
	context.init();

	Config main_config("settings");
	GameManager game_manager(main_config.getValue("rom_path"));

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