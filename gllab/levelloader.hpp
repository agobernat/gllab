#pragma once
#include <string>

class LevelLoader
{
public:
	LevelLoader();
	std::vector<std::vector<int>> loadFromFile(const std::string& filename);

private:

};



