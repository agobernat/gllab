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
    //glGenBuffers(1, &EBO);
    

  

    glBindVertexArray(this->quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (size - 1) * (size - 1) * 8, vertexarray, GL_DYNAMIC_DRAW);

    

    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * (size - 1) * (size - 1) * 6, indexarray, GL_DYNAMIC_DRAW);


    


    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glPatchParameteri(GL_PATCH_VERTICES, 4);

    // texture coord attribute
    //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    //glEnableVertexAttribArray(1);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}


void Terrain::draw(glm::vec3 pos, glm::vec3 axis, float angle, glm::mat4 view, unsigned int texture, double time) {

    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    // make sure to initialize matrix to identity matrix first
    // glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    
    model = glm::rotate(model, glm::radians(angle), axis);
    model = glm::scale(model, glm::vec3(4.0f, 4.0f, 1.0f));
    model = glm::translate(model, pos);
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


    unsigned int hmapsizeLoc = glGetUniformLocation(shader.ID, "hmapsize");
    glUniform1i(hmapsizeLoc, size);

    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);




    shader.setVec3("lightPos", glm::vec3(glm::sin(time / 20) * 50, 0.0f, glm::cos(time / 20) * 50));
    shader.setInt("hmap", 0);
    shader.setInt("texture1", 1);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hmap);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);


    glBindVertexArray(this->quadVAO);
    
    glDrawArrays(GL_PATCHES, 0, 4 * (size - 1) * (size - 1));

    //glDrawElements(GL_TRIANGLES, (size - 1) * (size - 1) * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Terrain::allocvertexarray(unsigned int size) {
    if (vertexarray != nullptr)
    {
        return;
    }
    vertexarray = new float[(size - 1) * (size - 1) * 8];
    //indexarray = new unsigned int[(size - 1) * (size - 1) * 6];
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps

    
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, hmapsize, hmapsize, 0, GL_RED, GL_FLOAT, hmap);
    glGenerateMipmap(GL_TEXTURE_2D);
   
    
    this->hmap = texture;
}


void Terrain::loadHmapFromImage(unsigned int texture) {
    this->hmap = texture;

}



void Terrain::genvertex(float* vert, unsigned int* ind, unsigned int size) {
    const unsigned int triglen = 6;
    
    for (size_t i = 0; i < size - 1; i++)
    {
        for (size_t j = 0; j < size - 1; j++)
        {
            vert[8 * (i * (size - 1) + j)] = float(i);
            vert[8 * (i * (size - 1) + j) + 1] = float(j);

            vert[8 * (i * (size - 1) + j) + 2] = float(i);
            vert[8 * (i * (size - 1) + j) + 3] = float(j + 1);

            vert[8 * (i * (size - 1) + j) + 4] = float(i + 1);
            vert[8 * (i * (size - 1) + j) + 5] = float(j);

            vert[8 * (i * (size - 1) + j) + 6] = float(i + 1);
            vert[8 * (i * (size - 1) + j) + 7] = float(j + 1);
        }
    }
   //0, 1, 2
   //1, 2, 3 
    
    //for (size_t i = 0; i < size - 1; i++)
    //{
    //    for (size_t j = 0; j < size - 1; j++)
    //    {
    //        ind[triglen * (i * (size - 1) + j)] = i * size + j;
    //        ind[triglen * (i * (size - 1) + j) + 1] = i * size + j + 1;
    //        ind[triglen * (i * (size - 1) + j) + 2] = (i + 1) * size + j;
    //
    //        ind[triglen * (i * (size - 1) + j) + 3] = i * size + j + 1;
    //        ind[triglen * (i * (size - 1) + j) + 4] = (i + 1) * size + j + 1;
    //        ind[triglen * (i * (size - 1) + j) + 5] = (i + 1) * size + j;
    //    }
    //}

}
