#pragma once
#include <math.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"



class TerrainGen
{
public:
    TerrainGen(Shader& shader);
    ~TerrainGen();
    void init();
    void draw(glm::vec3 pos, glm::vec3 axis, float angle, glm::mat4 view, unsigned int texture, glm::mat4 model = glm::mat4(1.0f));



private:
    unsigned int quadVAO;
    Shader shader;
};

TerrainGen::TerrainGen(Shader& shader) : shader(shader) { init(); }

TerrainGen::~TerrainGen()
{
}

void TerrainGen::init() {
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
         0.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
         0.0f,  1.0f, 0.0f,  1.0f, 1.0f
    };

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);


    glBindVertexArray(this->quadVAO);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}


void TerrainGen::draw(glm::vec3 pos, glm::vec3 axis, float angle, glm::mat4 view, unsigned int texture, glm::mat4 model) {

    shader.use();
    // make sure to initialize matrix to identity matrix first
    // glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(angle), axis);
    
    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
    
    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
    unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);
    
    

    
    
    //shader.setInt("texture", 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);


    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}
