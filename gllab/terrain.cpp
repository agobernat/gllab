#include "terrain.hpp"

#include <math.h>
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "macros.hpp"

Terrain::Terrain(Shader& shader, int size) : shader(shader), size(size) { 
    vertexarray = nullptr;
    init(); 
    
}

Terrain::~Terrain()
{
    delete[] vertexarray;
}


void Terrain::init() {
    // configure VAO/VBO
    allocvertexarray(size);

    unsigned int VBO, EBO;
    glGenVertexArrays(1, &this->quadVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    

  

    glBindVertexArray(this->quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * size * 2, vertexarray, GL_DYNAMIC_DRAW);

    

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * (size - 1) * (size - 1) * 6, indexarray, GL_DYNAMIC_DRAW);


    


    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // texture coord attribute
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}


void Terrain::draw(glm::vec3 pos, glm::vec3 axis, float angle, glm::mat4 view, unsigned int texture) {

    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    // make sure to initialize matrix to identity matrix first
    // glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    
    model = glm::rotate(model, glm::radians(angle), axis);
    model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
    model = glm::translate(model, pos);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

    // retrieve the matrix uniform locations
    GLuint hmapsizeLoc = glGetUniformLocation(shader.ID, "hmapsize");
    //unsigned int hmapsizeLoc = glGetUniformLocation(shader.ID, "heightmapsize");


    unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
    unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
    // pass them to the shaders (3 different ways)
    glUniform1i(hmapsizeLoc, size);
    //glUniform1fv(hmapLoc, hmapsize * hmapsize, hmap);


    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    shader.setInt("texture1", 0);
    shader.setInt("hmap", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, hmap);


    glBindVertexArray(this->quadVAO);
    glDrawElements(GL_TRIANGLES, (size - 1) * (size - 1) * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Terrain::allocvertexarray(unsigned int size) {
    if (vertexarray != nullptr)
    {
        return;
    }
    vertexarray = new float[size * size * 2];
    indexarray = new unsigned int[(size - 1) * (size - 1) * 6];
    genvertex(vertexarray, indexarray, size);

    /*for (size_t i = 0; i < (size - 1) * (size - 1) * 6; i++)
    {
        std::cout << indexarray[i] << ", ";

        if (i % (6) == (6 - 1))
        {
            std::cout << std::endl;
        }
    }*/
}

void Terrain::loadHmapAsTexture(float* hmap, int hmapsize) {

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps

    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, hmapsize, hmapsize, 0, GL_RED, GL_FLOAT, hmap);
    glGenerateMipmap(GL_TEXTURE_2D);
   
    
    this->hmap = texture;
}



/*void Terrain::genvertex(float* vert, int size) {
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
    
}*/


void Terrain::genvertex(float* vert, unsigned int* ind, unsigned int size) {
    const unsigned int triglen = 6;

    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            vert[2 * (i * size + j)] = float(i);
            vert[2 * (i * size + j) + 1] = float(j);
        }
    }
   //0, 1, 2
   //1, 2, 3 
    
    for (size_t i = 0; i < size - 1; i++)
    {
        for (size_t j = 0; j < size - 1; j++)
        {
            ind[triglen * (i * (size - 1) + j)] = i * size + j;
            ind[triglen * (i * (size - 1) + j) + 1] = i * size + j + 1;
            ind[triglen * (i * (size - 1) + j) + 2] = (i + 1) * size + j;

            ind[triglen * (i * (size - 1) + j) + 3] = i * size + j + 1;
            ind[triglen * (i * (size - 1) + j) + 4] = (i + 1) * size + j + 1;
            ind[triglen * (i * (size - 1) + j) + 5] = (i + 1) * size + j;
        }
    }

}
