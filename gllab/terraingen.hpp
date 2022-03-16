#pragma once
#include "OpenSimplexNoise/OpenSimplexNoise.h"

class TerrainGen
{
	

public:
	TerrainGen();
	TerrainGen(long long int seed);
	~TerrainGen();

	void GenerateHmap(float* arr, int fromX, int fromY, unsigned int X, unsigned int Y);

private:
	OpenSimplexNoise::Noise osnoise;
};

