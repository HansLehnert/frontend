#pragma once

#include <map>
#include <string>
#include <curl/curl.h>

#include <rapidxml/rapidxml.hpp>
#include "Config.h"

class Scrapper {
public:
	void scrap(std::map<std::string, Config>*, std::map<std::string, Config>*);
private:
	int getFile(std::string, std::string*, CURL*);
	int saveFile(std::string, std::string, CURL*);

	std::string cleanName(std::string);
};