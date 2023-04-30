#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include "levelloader.hpp"

#include <bitset>

LevelLoader::LevelLoader()
{
}


std::vector<std::pair<Transform, unsigned int>> LevelLoader::loadFromFile(const std::string& filename) {
    std::vector<std::pair<Transform, unsigned int>> tiles;
    
    std::ifstream f(filename.c_str());
    f.exceptions(std::ios::badbit);

    

    Transform transform;
    int i = 0;
    for (std::string line; std::getline(f, line); i++)
    {
        int j = 0;
        std::stringstream linestream;
        linestream.str(line + delimiter);
        for (std::string tilestr; std::getline(linestream, tilestr, delimiter); j++)
        {
            if (tilestr == "-1")
            {
                continue;
            }
            unsigned long transformflags = 0;
            unsigned int tile = std::stoul(tilestr);

            transformflags |= tile & FLIPPED_HORIZONTALLY_FLAG;
            transformflags |= tile & FLIPPED_VERTICALLY_FLAG;
            transformflags |= tile & FLIPPED_DIAGONALLY_FLAG;

            tile &= ~(FLIPPED_HORIZONTALLY_FLAG |
                FLIPPED_VERTICALLY_FLAG |
                FLIPPED_DIAGONALLY_FLAG |
                ROTATED_HEXAGONAL_120_FLAG);

            Transform transform;
            transform.translate = glm::translate(transform.translate, glm::dvec3(static_cast<double>(j), -static_cast<double>(i) + 30, 0.0f));

            switch (transformflags)
            {
            case(0b0000'0000'0000'0000'0000'0000'0000'0000):
                std::cout << "000" << std::endl;
                break;
            case(0b0010'0000'0000'0000'0000'0000'0000'0000):
                transform.scale = glm::scale(transform.scale, glm::dvec3(1.0, 1.0, -1.0));
                transform.rotation = glm::rotate(transform.rotation, 3.1415 * (1.0 / 2.0), glm::dvec3(0.0, 0.0, 1.0));
                std::cout << "001" << std::endl;
                break;
            case(0b0100'0000'0000'0000'0000'0000'0000'0000):
                transform.scale = glm::scale(transform.scale, glm::dvec3(1.0, 1.0, -1.0));
                std::cout << "010" << std::endl;
                break;
            case(0b0110'0000'0000'0000'0000'0000'0000'0000):
                transform.rotation = glm::rotate(transform.rotation, 3.1415 * (1.0 / 2.0), glm::dvec3(0.0, 0.0, 1.0));
                std::cout << "011" << std::endl;
                break;
            case(0b1000'0000'0000'0000'0000'0000'0000'0000):
                transform.scale = glm::scale(transform.scale, glm::dvec3(1.0, 1.0, -1.0));
                transform.rotation = glm::rotate(transform.rotation, 3.1415 * (2.0 / 2.0), glm::dvec3(0.0, 0.0, 1.0));
                std::cout << "100" << std::endl;
                break;
            case(0b1010'0000'0000'0000'0000'0000'0000'0000):
                transform.rotation = glm::rotate(transform.rotation, 3.1415 * (3.0  / 2.0), glm::dvec3(0.0, 0.0, 1.0));
                std::cout << "101" << std::endl;
                break;
            case(0b1100'0000'0000'0000'0000'0000'0000'0000):
                transform.rotation = glm::rotate(transform.rotation, 3.1415 * (2.0 / 2.0), glm::dvec3(0.0, 0.0, 1.0));
                std::cout << "110" << std::endl;
                break;
            case(0b1110'0000'0000'0000'0000'0000'0000'0000):
                transform.scale = glm::scale(transform.scale, glm::dvec3(1.0, 1.0, -1.0));
                transform.rotation = glm::rotate(transform.rotation, 3.1415 * (3.0 / 2.0), glm::dvec3(0.0, 0.0, 1.0));
                std::cout << "111" << std::endl;
                break;
            default:
                break;
            }
            
            tiles.push_back(std::pair<Transform, int>(transform, tile));
        }

    }

    return tiles;
}