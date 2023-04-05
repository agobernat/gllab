#pragma once
#include "gameobject.hpp"
#include <string>

class LevelLoader
{
public:
	LevelLoader();
	std::vector<std::pair<Transform, unsigned int>> loadFromFile(const std::string& filename);

private:
	const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
	const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
	const unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;
	const unsigned ROTATED_HEXAGONAL_120_FLAG = 0x10000000;

	const char delimiter = ',';
};



