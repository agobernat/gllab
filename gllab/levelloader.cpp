#include <vector>
#include <fstream>
#include <iostream>
#include "levelloader.hpp"

LevelLoader::LevelLoader()
{
}


std::vector<std::vector<int>> LevelLoader::loadFromFile(const std::string& filename) {
    std::vector<std::vector<int>> levelmat;
    int rows, cols;
    
    std::ifstream f(filename.c_str());
    f.exceptions(std::ios::failbit | std::ios::badbit);

    f >> rows >> cols;

    for (size_t i = 0; i < rows; i++)
    {
        std::vector<int> row;
        for (size_t j = 0; j < cols; j++)
        {
            int tile;
            f >> tile;
            row.push_back(tile);
        }
        levelmat.push_back(row);
    }
    try {
       
    }
    catch (std::ifstream::failure&) {
        return std::vector<std::vector<int>>();
    }


    return levelmat;
}