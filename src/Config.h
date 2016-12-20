#pragma once

#include <map>
#include <string>

class Config {
public:
	Config(std::string);
	
	int load();
	int write();

	void setValue(std::string, std::string);
	std::string getValue(std::string, int* = NULL);
private:
	std::string filename;
	std::map<std::string, std::string> settings;
};