#pragma once

#include "sprite.hpp"
#include <glm/glm.hpp>
#include "model.hpp"

struct Transform {
	Transform() {
		translate = glm::dmat4(1.0);
		scale = glm::dmat4(1.0);
		rotation = glm::dmat4(1.0);
	}
	glm::dmat4 translate, scale, rotation;
};
class GameModel;

class GameObject
{

private:
	
	Transform transform;
	const GameModel* modelptr;
	

public:

	GameObject();
	GameObject(const GameModel& model);

	void scale(glm::dvec3 scale);
	void translate(glm::dvec3 translate);
	//void setRotation(glm::dvec3 rot);
	void rotate(double angle, glm::dvec3 axis);
	void draw(const Camera& camera) const;
	//glm::mat4 makeTransform(const Transform& transform, glm::mat4 model = glm::mat4(1.0));
	//void setSprite(Sprite sprite);

};


