#pragma once

#include <vector>
#include <string>

#include "core/Object.h"
#include "objects/Text.h"
#include "Message.h"

class ItemList : public Object {
public:
	ItemList(std::vector<std::string>);

	//virtual void sendMessage(Message);
private:
	std::vector<std::string> items;
	std::vector<std::string>::iterator selection;
	Text item_text;
};