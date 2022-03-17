#pragma once
#include "OpenSimplexNoise/OpenSimplexNoise.h"

class TerrainGen
{
	

public:
	TerrainGen();
	TerrainGen(long long int seed);
	~TerrainGen();

	void GenerateHmap(float* arr, long long int fromX, long long int fromY, unsigned long long int x, unsigned long long int y);

private:
	OpenSimplexNoise::Noise osnoise;
};

