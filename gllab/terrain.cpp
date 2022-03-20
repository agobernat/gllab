#include "terrain.hpp"

#include <math.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "macros.hpp"


Terrain::Terrain(Shader& shader, unsigned int size, unsigned int res, GLuint texture, long long int seed) : shader(shader), size(size), res(res), texture(texture), generator(seed) {
    vertexarray = nullptr;
    //init(); 
    
}

Terrain::Terrain(Shader& shader, unsigned int size, unsigned int res, GLuint texture) : shader(shader), size(size), res(res), texture(texture), generator() {
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


void Terrain::init() {
    // configure VAO/VBO
    //allocvertexarray(size);
    //
    //unsigned int VBO;
    //glGenVertexArrays(1, &this->quadVAO);
    //glGenBuffers(1, &VBO);
    ////glGenBuffers(1, &EBO);
    //
    //
    //
    //
    //glBindVertexArray(this->quadVAO);
    //
    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (size - 1) * (size - 1) * 8, vertexarray, GL_DYNAMIC_DRAW);
    //
    //
    //
    ////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    ////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * (size - 1) * (size - 1) * 6, indexarray, GL_DYNAMIC_DRAW);
    //
    //
    //
    //
    //
    //// position attribute
    //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);
    //
    //glPatchParameteri(GL_PATCH_VERTICES, 4);
    //
    //// texture coord attribute
    ////glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    ////glEnableVertexAttribArray(1);
    //
    //
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);

}


void Terrain::draw(double time, Camera camera) {

    for (Chunk* chunk : chunklist)
    {
        chunk->draw(time, camera, shader, texture);
    }
}

//void Terrain::draw(glm::vec3 pos, glm::vec3 axis, float angle, glm::mat4 view, unsigned int texture, double time, glm::vec3 cameraPos) {
//
//    shader.use();
//    glm::mat4 model = glm::mat4(1.0f);
//    // make sure to initialize matrix to identity matrix first
//    // glm::mat4 view = glm::mat4(1.0f);
//    glm::mat4 projection = glm::mat4(1.0f);
//
//    
//    model = glm::rotate(model, glm::radians(angle), axis);
//    model = glm::scale(model, glm::vec3(128.0f, 128.0f, 1.0f));
//    model = glm::translate(model, pos);
//    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
//
//
//    unsigned int hmapsizeLoc = glGetUniformLocation(shader.ID, "hmapsize");
//    glUniform1i(hmapsizeLoc, size * 16);
//
//    shader.setMat4("projection", projection);
//    shader.setMat4("view", view);
//    shader.setMat4("model", model);
//
//
//    shader.setVec3("viewPos", cameraPos);
//    shader.setVec3("lightPos", glm::vec3(glm::sin(time / 10) * 100, 0.0f, glm::abs(glm::cos(time / 10) * 100)));
//    shader.setInt("hmap", 0);
//    shader.setInt("texture1", 1);
//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, hmap);
//    
//    glActiveTexture(GL_TEXTURE1);
//    glBindTexture(GL_TEXTURE_2D, texture);
//
//
//    glBindVertexArray(this->quadVAO);
//    
//    glDrawArrays(GL_PATCHES, 0, 4 * (size - 1) * (size - 1));
//
//    //glDrawElements(GL_TRIANGLES, (size - 1) * (size - 1) * 6, GL_UNSIGNED_INT, 0);
//    glBindVertexArray(0);
//}


