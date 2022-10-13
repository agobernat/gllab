#include "gameobject.hpp"



GameObject::GameObject() 
{
	
}

GameObject::GameObject(const GameModel& model) {
	modelptr = &model;
}


void GameObject::scale(glm::dvec3 scale)
{
	transform.scale = glm::scale(transform.scale, scale);
}
void GameObject::translate(glm::dvec3 pos)
{
	transform.translate = glm::translate(transform.translate, pos);
}
void GameObject::rotate(double angle, glm::dvec3 axis) {
	transform.rotation = glm::rotate(transform.rotation, angle, axis);
}

/*void GameObject::setSprite(Sprite sprite) {
	this->sprite = sprite;
}*/

void GameObject::draw(const Camera& camera) const
{
	modelptr->draw(camera, transform);
}

