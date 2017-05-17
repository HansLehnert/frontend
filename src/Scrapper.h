#pragma once

#include <map>
#include <string>
#include <curl/curl.h>

#include "Config.h"

void scrapDatFiles(std::map<std::string, Config>*, std::map<std::string, Config>*);
void scrapTheGamesDB(std::map<std::string, Config>*, std::map<std::string, Config>*);
void scrapArcadeItalia(std::map<std::string, Config>*, std::map<std::string, Config>*);

bool getFile(std::string, std::string*, CURL*);
bool saveFile(std::string, std::string, CURL*);

std::string cleanName(std::string);