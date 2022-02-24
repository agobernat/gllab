#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include "sprite.hpp"

class GameObject
{

private:
	int x, y, h, w, dx, dy, dh, dw;
	unsigned int texture;
	double rotation;
	Sprite sprite;
	

public:
	GameObject();
	~GameObject();
	void setTexture(unsigned int texture);
	void setSize(int h, int w);
	void setPosition(int x, int y);	
	void setRotation(double rad);
	void rotate(double rad);
	void draw();
	void setSprite(Sprite sprite);

};

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}



#endif