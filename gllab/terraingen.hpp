#pragma once
#include "OpenSimplexNoise/OpenSimplexNoise.h"

class TerrainGen
{
	

public:
	TerrainGen();
	TerrainGen(long long int seed);
	~TerrainGen();

	void GenerateHmap(float* arr, int size);

private:
	OpenSimplexNoise::Noise osnoise;
};

