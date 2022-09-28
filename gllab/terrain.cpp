#include "terrain.hpp"

#include <math.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


Terrain::Terrain(const Shader& shader, unsigned int size, unsigned int res, GLuint texture, long long int seed) : shader(shader), size(size), res(res), texture(texture), generator(seed) {
    vertexarray = nullptr;
    //init(); 
    
}

Terrain::Terrain(const Shader& shader, unsigned int size, unsigned int res, GLuint texture) : shader(shader), size(size), res(res), texture(texture), generator() {
    vertexarray = nullptr;
    //init();

}

Terrain::~Terrain()
{
    delete[] vertexarray;
}

void Terrain::loadChunk(int x, int y) {
    Chunk* chunk = new Chunk(*this, size, res, x, y);
    chunklist.push_back(chunk);
}

void Terrain::unloadChunk(int x, int y) {
    
    for (auto p = chunklist.begin(); p != chunklist.end(); ++p)
    {
        if ((*p)->Xcoord == x && (*p)->Ycoord == y)
        {
            delete *p;
            chunklist.erase(p);
            break;
        }
    }

}

void Terrain::draw(double time, Camera camera) {

    for (Chunk* chunk : chunklist)
    {
        chunk->draw(time, camera, shader, texture);
    }
}


