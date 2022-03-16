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

void TerrainGen::GenerateHmap(float* arr, int fromX, int fromY, unsigned int x, unsigned int y) {
    const float flatness = 20.0f;
    const float amplification = 10.0f;
    const int harmonics = 4;
    for (size_t i = 0; i < x; i++)
    {
        for (size_t j = 0; j < y; j++)
        {
            arr[i * y + j] = amplification * (float)osnoise.eval((fromX + i) / flatness, (fromY + j) / flatness);
            for (size_t k = 1; k < harmonics; k++)
            {
                arr[i * y + j] += amplification / k * (float)osnoise.eval((fromX + i) /  flatness * k, (fromY + j) /  flatness * k);
            }
            
        }
    }
}