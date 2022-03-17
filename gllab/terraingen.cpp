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

void TerrainGen::GenerateHmap(float* arr, long long int fromX, long long int fromY, unsigned long long int x, unsigned long long int y) {
    fromX += 2147483647;
    fromY += 2147483647;
    const double flatness = 20.0;
    const double amplification = 20.0;
    const int harmonics = 7;
    for (size_t i = 0; i < x; i++)
    {
        for (size_t j = 0; j < y; j++)
        {
            float height = (float)osnoise.eval((fromX + i) / flatness, (fromY + j) / flatness);
            if (height < 0.0f)
            {
                height *= 0.1f;
            }
            else if(height > 0.0f)
            {
                height *= 3.0f;
            }
            arr[i * y + j] = amplification * height;
            for (size_t k = 1; k < harmonics; k++)
            {
                float height = (float)osnoise.eval((fromX + i) / flatness * k, (fromY + j) / flatness * k);
                if (height < 0.0f)
                {
                    height *= 0.8f;
                }
                else if (height > 0.0f)
                {
                    height *= 3.4f;
                }
                arr[i * y + j] += height;
            }
            
        }
    }
}