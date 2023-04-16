#include "debugdrawer.hpp"

DebugDrawer::DebugDrawer() : btIDebugDraw(), vertices{ 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f} {
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &vbo);

    auto& shaderManager = Globals::shaderManager;
    shaderManager.loadShader("lineshader", "lineshader.vert", "lineshader.frag");
    this->shader = &shaderManager.getShader("lineshader");
    this->m = glm::mat4(1.0);
    this->v = glm::mat4(1.0);
    this->p = glm::mat4(1.0);
}

void DebugDrawer::setMVP(glm::mat4 m, glm::mat4 v, glm::mat4 p)
{
    this->m = m;
    this->v = v;
    this->p = p;
}

void DebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
    vertices[0] = from.getX();
    vertices[1] = from.getY();
    vertices[2] = from.getZ();
    vertices[3] = to.getX();
    vertices[4] = to.getY();
    vertices[5] = to.getZ();

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->vao);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    

    shader->use();
    shader->setMat4("model", m);
    shader->setMat4("view", v);
    shader->setMat4("proj", p);
    shader->setVec4("dbgColor", glm::vec4(color.getX(), color.getY(), color.getZ(), 1.0f));
    glDrawArrays(GL_LINE_STRIP, 0, 2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

/*

void Sprite::initRenderData()
{

   
    


    


    // position attribute
    
    // texture coord attribute
    





}

void Sprite::Draw(glm::vec3 position, unsigned int texture, glm::mat4 view)
{
    shader.use();
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    float rot = position.y * 100;
    model = glm::translate(model, glm::vec3(-position.x, position.y, position.z));
    model = glm::rotate(model, glm::radians(rot), glm::vec3(1.0f, 0.0f, 0.0f));

    projection = glm::perspective(glm::radians(45.0f), (float)Static::SCR_WIDTH / (float)Static::SCR_HEIGHT, 0.1f, 10000.0f);
    // retrieve the matrix uniform locations
    unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
    unsigned int viewLoc = glGetUniformLocation(shader.ID, "view");
    // pass them to the shaders (3 different ways)
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model)); \
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
    // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    //shader.setInt("texture", 0);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);


    glBindVertexArray(this->quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}*/