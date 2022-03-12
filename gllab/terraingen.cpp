#include "terraingen.hpp"
#include "include/OpenSimplexNoise/OpenSimplexNoise.h"

#include <iostream>
#include <random>
#include <cmath>

OpenSimplexNoise::Noise init() {

    std::mt19937_64 e2(time(nullptr));
    std::uniform_int_distribution<long long int> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));
    return OpenSimplexNoise::Noise(dist(e2));
}

OpenSimplexNoise::Noise init(long long int seed) {

    std::mt19937_64 e2(seed);
    std::uniform_int_distribution<long long int> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));
    return OpenSimplexNoise::Noise(dist(e2));
}


TerrainGen::TerrainGen() : osnoise(init()) { }


TerrainGen::TerrainGen(long long int seed) : osnoise(init(seed)) { }


TerrainGen::~TerrainGen()
{
}

void TerrainGen::GenerateHmap(float* arr, int size) {

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            arr[i * size + j] = 8 * (float)osnoise.eval(i / 30.0, j / 30.0);
        }
    }
}