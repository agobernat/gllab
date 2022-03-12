#include "terraingen.hpp"

#include <math.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

TerrainGen::TerrainGen(Shader& shader, int size) : shader(shader), size(size) { 
    vertexarray = nullptr;
    init(); 
    
}

TerrainGen::~TerrainGen()
{
    delete[] vertexarray;
}


void TerrainGen::init() {
    // configure VAO/VBO
    unsigned int VBO;
    allocvertexarray(size);

    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);

    //glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (size - 1) * (size - 1) * 30, vertexarray, GL_DYNAMIC_DRAW);


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


void TerrainGen::draw(glm::vec3 pos, glm::vec3 axis, float angle, glm::mat4 view, unsigned int texture, float* hmap, int hmapsize) {

    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    // make sure to initialize matrix to identity matrix first
    // glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::translate(model, pos);
    model = glm::rotate(model, glm::radians(angle), axis);

    projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

    // retrieve the matrix uniform locations
    unsigned int hmapLoc = glGetUniformLocation(shader.ID, "heightmap");
    unsigned int hmapsizeLoc = glGetUniformLocation(shader.ID, "heightmapsize");


    unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
    unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
    // pass them to the shaders (3 different ways)
    glUniform1i(hmapsizeLoc, hmapsize);
    glUniform1fv(hmapLoc, hmapsize * hmapsize, hmap);


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
    glDrawArrays(GL_TRIANGLES, 0, (size - 1) * (size - 1) * 6);
    glBindVertexArray(0);
}

void TerrainGen::allocvertexarray(int size) {
    if (vertexarray != nullptr)
    {
        return;
    }
    vertexarray = new float[(size - 1) * (size - 1) * 30];
    genvertex(vertexarray, size);
    /*for (size_t i = 0; i < (size - 1) * (size - 1) * 30; i++)
    {
        std::cout << vertexarray[i] << ", ";
        if (i % 5 == 4)
        {
            std::cout << std::endl;
        }
        if (i % (30) == (30 - 1))
        {
            std::cout << std::endl;
        }
    }*/
}

void TerrainGen::genvertex(float* vert, int size) {
    const int triglen = 15 * 2;

    for (size_t i = 0; i < size - 1; i++)
    {
        for (size_t j = 0; j < size - 1; j++)
        {
            vert[triglen * (i * (size - 1) + j)] = float(i);
            vert[triglen * (i * (size - 1) + j) + 1] = float(j);
            vert[triglen * (i * (size - 1) + j) + 2] = 0.0f;

            vert[triglen * (i * (size - 1) + j) + 3] = 0.0f;
            vert[triglen * (i * (size - 1) + j) + 4] = 0.0f;

            vert[triglen * (i * (size - 1) + j) + 5] = float(i) + 1.0f;
            vert[triglen * (i * (size - 1) + j) + 6] = float(j);
            vert[triglen * (i * (size - 1) + j) + 7] = 0.0f;
                ;               
            vert[triglen * (i * (size - 1) + j) + 8] = 1.0f;
            vert[triglen * (i * (size - 1) + j) + 9] = 0.0f;
                                
            vert[triglen * (i * (size - 1) + j) + 10] = float(i);
            vert[triglen * (i * (size - 1) + j) + 11] = float(j) + 1.0f;
            vert[triglen * (i * (size - 1) + j) + 12] = 0.0f;
                                
            vert[triglen * (i * (size - 1) + j) + 13] = 0.0f;
            vert[triglen * (i * (size - 1) + j) + 14] = 1.0f;
                          
                                
                             
            vert[triglen * (i * (size - 1) + j) + 15] = float(i) + 1.0f;
            vert[triglen * (i * (size - 1) + j) + 16] = float(j);
            vert[triglen * (i * (size - 1) + j) + 17] = 0.0f;
                                
            vert[triglen * (i * (size - 1) + j) + 18] = 1.0f;
            vert[triglen * (i * (size - 1) + j) + 19] = 0.0f;
                                
            vert[triglen * (i * (size - 1) + j) + 20] = float(i) + 1.0f;
            vert[triglen * (i * (size - 1) + j) + 21] = float(j) + 1.0f;
            vert[triglen * (i * (size - 1) + j) + 22] = 0.0f;
                                
            vert[triglen * (i * (size - 1) + j) + 23] = 1.0f;
            vert[triglen * (i * (size - 1) + j) + 24] = 1.0f;
                                
            vert[triglen * (i * (size - 1) + j) + 25] = float(i);
            vert[triglen * (i * (size - 1) + j) + 26] = float(j) + 1.0f;
            vert[triglen * (i * (size - 1) + j) + 27] = 0.0f;
                               
            vert[triglen * (i * (size - 1) + j) + 28] = 0.0f;
            vert[triglen * (i * (size - 1) + j) + 29] = 1.0f;
        }
    }
    
}
