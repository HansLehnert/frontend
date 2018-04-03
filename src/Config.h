#pragma once

#include <map>
#include <string>

class Config {
public:
	Config(std::string filename);
	~Config();

	bool load();
	bool write();

	std::string& operator[](const std::string& key);
private:
	std::string filename;
	std::map<std::string, std::string> settings;
};
