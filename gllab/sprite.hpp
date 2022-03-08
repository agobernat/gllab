#ifndef _SPRITE_
#define _SPRITE_


#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"



class Sprite
{
	
public:
	//Sprite();
	Sprite(Shader &shader);
	~Sprite();

	void Draw(glm::vec3 position, unsigned int texture, glm::mat4 view);

	

private:
	Shader shader;
	float* vertexmatrix;
	unsigned int quadVAO;
	void initRenderData();
};





#endif // !_SPRITE_
