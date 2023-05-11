#pragma once
#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "static.hpp"

class Camera
{
public:
	Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up);
	Camera();
	~Camera();

	void move(float x, float y);
	const glm::mat4 view() const;
	const glm::mat4 projection() const;
	static Camera getDefault();
	void linearMove(glm::vec3 pos, glm::vec3 front, double duration);
	void tick(double dt);


	glm::vec3 front, pos, up;
	int lastX, lastY;
private:
	float yaw;
	float pitch;
	float fov;

	double moveRemainingDuration;
	glm::vec3 frontGoal, posGoal;

	glm::mat4 proj;

	
};
