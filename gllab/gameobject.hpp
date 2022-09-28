#pragma once

#include "sprite.hpp"
#include <glm/glm.hpp>

class GameObject
{

private:
	glm::dvec3 position, scale, rotation, translate;
	//Sprite sprite;
	

public:
	GameObject();
	~GameObject();

	void setScale(glm::dvec3 scale);
	void setPosition(glm::dvec3 pos);
	//void setRotation(glm::dvec3 rot);
	void rotate(double rot, glm::dvec3 axis);
	void draw();
	//void setSprite(Sprite sprite);

};

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

