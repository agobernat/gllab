#pragma once


#include "shader.h"



class Terrain
{
public:
    Terrain(Shader& shader, int size);
    ~Terrain();
    void init();
    void draw(glm::vec3 pos, glm::vec3 axis, float angle, glm::mat4 view, unsigned int texture, double time, glm::vec3 cameraPos);
    void loadHmapAsTexture(float* hmap, int hmapsize);
    void loadHmapFromImage(unsigned int texture);
    



private:
    unsigned int quadVAO;
    Shader shader;
    int size;

    GLuint hmap;

    void genvertex(float* vert, unsigned int* ind, unsigned int size);
    void allocvertexarray(unsigned int size);

    float* vertexarray;
    unsigned int* indexarray;
};



