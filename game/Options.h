#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
#include <fstream>
#include <unordered_map>

class Options {
public:
	std::unordered_map<std::string, float> options;

	Options () {}

	void loadOptions (std::string filename) {
		std::ifstream file(filename.c_str());

		std::string optionName;
		float optionValue;

		while (file >> optionName && file >> optionValue) {
			options[optionName] = optionValue;
		}

		file.close(); 
	}

	float operator [] (std::string name) {
		if (options.find(name) != options.end())
			return options[name];
		else
			return 1;
	}
};

#endif