#pragma once

#include <string>
#include <vector>
#include <map>

#include "core/Object.h"

#include "Config.h"
#include "Message.h"
#include "objects/Image.h"
#include "objects/Text.h"

class GameManager : public Object {
public:
	GameManager(std::string);
	int launchGame(std::string);

	virtual void sendMessage(Message);

	std::vector<std::string> getGameList();
private:
	void init(std::string);
	void updateUI();

	std::map<std::string, Config> game_list;
	std::map<std::string, Config> emulator_list;

	std::map<std::string, Config>::iterator selection;

	Image game_logo;
	Text game_title;
};