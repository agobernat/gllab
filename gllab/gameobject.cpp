#include "gameobject.hpp"
#include "macros.hpp"


void GameObject::setScale(glm::dvec3 scale)
{
	this->scale = scale;
}
void GameObject::setPosition(glm::dvec3 pos)
{
	this->position = pos;
}

void GameObject::setSprite(Sprite sprite) {
	this->sprite = sprite;
}

void GameObject::rotate(double rad, glm::dvec3 axis) {
	
}

void GameObject::draw()
{

}
