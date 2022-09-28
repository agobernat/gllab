#pragma once


#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader_t.h"
#include "static.hpp"


class Sprite
{
	
public:
	//Sprite();
	Sprite(const Shader &shader);
	~Sprite();

	void Draw(glm::vec3 position, unsigned int texture, glm::mat4 view);

	

private:
	const Shader shader;
	float* vertexmatrix;
	unsigned int quadVAO;
	void initRenderData();
};

