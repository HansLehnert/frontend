#include "objects/ItemList.h"

#include <vector>
#include <string>
#include <iostream>

#include "glm/glm.hpp"

ItemList::ItemList(std::vector<std::string> list) : items(list) {
	selection = items.begin();

	item_text.setContent(*selection);
	item_text.setScale(glm::vec2(0.05, 0.08));
	item_text.setSpacing(0.2);
}