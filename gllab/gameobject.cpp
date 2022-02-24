#include "gameobject.hpp"
#include "macros.hpp"

void GameObject::setTexture(unsigned int texture)
{
	this->texture = texture;
}
void GameObject::setSize(int h, int w)
{
	this->h = h;
	this->w = w;
}
void GameObject::setPosition(int x, int y)
{
	this->x = x;
	this->y = y;
}
void GameObject::setRotation(double rad)
{
	this->rotation = rad;
}

void GameObject::setSprite(Sprite sprite) {
	this->sprite = sprite;
}

void GameObject::rotate(double rad) {
	this->rotation = this->rotation + rad > 2 * PI ? this->rotation + rad - 2 * PI : this->rotation + rad;
}

void GameObject::draw()
{

}
