#pragma once

#include <string>
#include <vector>
#include <map>

#include "core/Object.h"

#include "Config.h"
#include "core/Text.h"
#include "objects/Image.h"
#include "objects/IconGrid.h"
#include "objects/Marquee.h"
#include "Context.h"

class GameManager {
public:
	GameManager(Context* p_context, Config* main_config);
	int launchGame(std::string);

	std::map<std::string, Config>* getGameList();
private:
	void init(std::string rom_path);
	void updateUI();

	Context* context;

	Config* config;

	std::map<std::string, Config> game_list;
	std::map<std::string, Config> emulator_list;
};
