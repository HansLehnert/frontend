#include "Config.h"

#include <fstream>
#include <string>
#include <iostream>

Config::Config(std::string file) : filename(file) {
	load();
}

bool Config::load() {
	std::ifstream config_file(filename);

	if (config_file.is_open()) {
		while (!config_file.eof()) {
			std::string line;
			std::getline(config_file, line);

			auto delimiter = line.find('=');
			std::string key = line.substr(0, delimiter);
			std::string value = line.substr(delimiter + 1);

			if (key.length() > 0 && value.length() > 0) {
				settings[key] = value;
			}
		}
		config_file.close();
		return true;
	}
	else {
		return false;
	}
}

bool Config::write() {
	std::ofstream config_file(filename);

	if (!config_file.is_open()) {
		return false;
	}

	auto pair = settings.begin();
	while (pair != settings.end()) {
		config_file << pair->first;
		config_file << "=";
		config_file << pair->second;
		config_file << std::endl;
		pair++;
	}

	config_file.close();
	return true;
}

void Config::setValue(std::string key, std::string value) {
	settings[key] = value;
}

std::string Config::getValue(std::string key, bool* success) {
	auto search = settings.find(key);
	if (search != settings.end()) {
		if (success != NULL)
			*success = true;
		return search->second;
	}
	else {
		if (success != NULL)
			*success = false;
		return std::string("");
	}
}

std::string Config::operator[](std::string key) {
	return getValue(key);
}