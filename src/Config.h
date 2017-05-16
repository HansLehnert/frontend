#pragma once

#include <map>
#include <string>

class Config {
public:
	Config(std::string);
	~Config();
	
	bool load();
	bool write();

	void setValue(std::string, std::string);


	std::string getValue(std::string, bool* = NULL);
	std::string operator[](std::string);
private:
	std::string filename;
	std::map<std::string, std::string> settings;
};