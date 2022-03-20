#pragma once
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "camera.hpp"
#include <shader_t.h>
#include "macros.hpp"
#include "terrain.hpp"

class Terrain;
class Chunk
{
public:
	Chunk(Terrain& terrain, unsigned int size, unsigned int res, int Xcoord, int Ycoord);
	~Chunk();
    void init();
    void draw(double time, Camera camera, Shader shader, GLuint texture);
    void loadHmapAsTexture(float* hmap, int hmapsize);


    float* hmap;
    GLuint hmaptexture;
    float* vertexarray;
    unsigned int VAO;

    unsigned int size, res;
    int Xcoord, Ycoord;
private:
    void genvertex(float* vert, unsigned int size);
    void allocvertexarray(unsigned int size);
    //glm::vec3 rotate;
    //glm::vec3 scale;
    glm::vec3 translate;
    glm::mat4 model;
};
