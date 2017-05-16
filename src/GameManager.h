#pragma once

#include <string>
#include <vector>
#include <map>

#include "core/Object.h"

#include "Config.h"
#include "objects/Image.h"
#include "objects/Text.h"
#include "Context.h"

class GameManager : public Object {
public:
	GameManager(Context*, Config*, std::string = "game_manager");
	int launchGame(std::string);

	virtual void handleEvent(Event&);

	std::vector<std::string> getGameList();
private:
	void init(std::string);
	void updateUI();

	Context* context;

	Config* config;

	std::map<std::string, Config> game_list;
	std::map<std::string, Config> emulator_list;

	std::map<std::string, Config>::iterator selection;

	Image game_logo;
	Text game_title;
};
