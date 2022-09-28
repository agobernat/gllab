#pragma once


#include "shader.h"
#include <list>
#include "terraingen.hpp"
#include "chunk.hpp"
#include "camera.hpp"


class Chunk;
class Terrain
{
    
public:
    Terrain(const Shader& shader, unsigned int size, unsigned int res, GLuint texture, long long int seed);
    Terrain(const Shader& shader, unsigned int size, unsigned int res, GLuint texture);
    ~Terrain();
    void init();
    void draw(double time, Camera camera);
    
    //void loadHmapFromImage(unsigned int texture);
    void loadChunk(int x, int y);
    void unloadChunk(int x, int y);
    TerrainGen generator;



private:
    
    GLuint texture;
    

    std::list<Chunk*> chunklist;
    std::list<void*> drawlist;

    //unsigned int quadVAO;
    const Shader& shader;
    unsigned int size, res;

    //GLuint hmap;

    //void genvertex(float* vert, unsigned int size);
    //void allocvertexarray(unsigned int size);

    float* vertexarray;
};



