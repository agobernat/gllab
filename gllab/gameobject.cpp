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
void GameObject::translate(glm::dvec3 translate)
{
	transform.translate = glm::translate(transform.translate, translate);
}

void GameObject::setTranslate(glm::dvec3 translate) {
	transform.translate = glm::translate(glm::dmat4(1.0), translate);
}

glm::dvec3 GameObject::getTransformVec() {
	return glm::dvec3(transform.translate[3][0], transform.translate[3][1], transform.translate[3][2]);
}

void GameObject::setRotation(double angle, glm::dvec3 rot) {
	transform.rotation = glm::rotate(glm::dmat4(1.0), angle, rot);
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

