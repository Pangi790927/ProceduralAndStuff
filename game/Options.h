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

		while (file >> optionName) {
			if (optionName.size() >= 1 && optionName[0] == '#') {
				std::string line;
				getline(file, line);
				std::cout << line << std::endl;
			}
			else if (file >> optionValue) {
				options[optionName] = optionValue;
			}
		}

		std::cout << *this << std::endl;

		file.close(); 
	}

	friend std::ostream& operator << (std::ostream &stream, Options &arg) {
		for (auto&& entry : arg.options) {
			std::cout << entry.first << " " << entry.second << std::endl;
		}

		return stream;
	}

	float operator [] (std::string name) {
		if (options.find(name) != options.end())
			return options[name];
		else
			return 1;
	}
};

#endif