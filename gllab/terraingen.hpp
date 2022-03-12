#pragma once


#include "shader.h"



class TerrainGen
{
public:
    TerrainGen(Shader& shader, int size);
    ~TerrainGen();
    void init();
    void draw(glm::vec3 pos, glm::vec3 axis, float angle, glm::mat4 view, unsigned int texture);
    void loadHmapAsTexture(float* hmap, int hmapsize);
    



private:
    unsigned int quadVAO;
    Shader shader;
    int size;

    GLuint hmap;

    void genvertex(float* vert, int size);
    void allocvertexarray(int size);

    float* vertexarray;
};



