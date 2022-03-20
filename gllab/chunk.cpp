#include "chunk.hpp"
#include "terraingen.hpp"





Chunk::Chunk(Terrain& terrain, unsigned int size, unsigned int res, int Xcoord, int Ycoord) : size(size), res(res), Xcoord(Xcoord), Ycoord(Ycoord) {
	hmap = new float[size * size * res * res];
    terrain.generator.GenerateHmap(hmap, (int)size * (int)res * Xcoord - Xcoord, (int)size * (int)res * Ycoord - Ycoord, (int)size * (int)res, (int)size * (int)res);
    loadHmapAsTexture(hmap, size * res);
    init();
}

Chunk::~Chunk() {
	delete[] hmap;
}



void Chunk::init() {

    allocvertexarray(size);

    unsigned int VBO;
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(this->VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (size - 1) * (size - 1) * 8, vertexarray, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glPatchParameteri(GL_PATCH_VERTICES, 4);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}




void Chunk::draw(double time, Camera camera, Shader shader, GLuint texture) {

    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
   
   
    //model = glm::rotate(model, glm::radians(angle), axis);
    model = glm::scale(model, glm::vec3((float)size * res / 2, (float)size * res / 2, 1.0f));
    model = glm::translate(model, glm::vec3(float(int(size) * Ycoord - Ycoord), float(int(size) * Xcoord - Xcoord), 0.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
   
   
    unsigned int hmapsizeLoc = glGetUniformLocation(shader.ID, "hmapsize");
    glUniform1i(hmapsizeLoc, size * res);
   
    shader.setMat4("projection", projection);
    shader.setMat4("view", camera.view());
    shader.setMat4("model", model);
   
   
    shader.setVec3("viewPos", camera.pos);
    shader.setVec3("lightPos", glm::vec3(glm::sin(time / 10) * 100, 0.0f, glm::abs(glm::cos(time / 10) * 100)));
    shader.setInt("hmap", 0);
    shader.setInt("texture1", 1);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hmaptexture);
   
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture);
   
   
    glBindVertexArray(this->VAO);
   
    glDrawArrays(GL_PATCHES, 0, 4 * (size - 1) * (size - 1));
   
    //glDrawElements(GL_TRIANGLES, (size - 1) * (size - 1) * 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

}

void Chunk::loadHmapAsTexture(float* hmap, int hmapsize) {

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps



    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, hmapsize, hmapsize, 0, GL_RED, GL_FLOAT, hmap);
    glGenerateMipmap(GL_TEXTURE_2D);


    this->hmaptexture = texture;
}

void Chunk::allocvertexarray(unsigned int size) {
    if (vertexarray != nullptr)
    {
        return;
    }
    vertexarray = new float[(size - 1) * (size - 1) * 8];
    genvertex(vertexarray, size);
}

void Chunk::genvertex(float* vert, unsigned int size) {

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

}