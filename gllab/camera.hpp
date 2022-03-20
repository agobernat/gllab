#pragma once
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up);
	Camera();
	~Camera();

	void move(float x, float y);
	glm::mat4 view();

	glm::vec3 front, pos, up;
	int lastX, lastY;
private:
	float yaw;
	float pitch;
	float fov;

	
};
