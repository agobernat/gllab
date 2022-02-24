#ifndef _SPRITE_
#define _SPRITE_

#include <glm/glm.hpp>


class Sprite
{
	static int screenheight, screenwidth;
public:
	Sprite();
	~Sprite();

	int x, y, h, w, dx, dy, dh, dw;
	void setTexture(unsigned int texture);
	void setScreenDimensions(int height, int width);
	void draw();
	void setSize(int h, int w);
	void setPosition(int x, int y);
	void setRotation(double rad);

private:

};

Sprite::Sprite()
{
	dx = 1; dy = 1; dh = 1; dw = 1;
}

Sprite::~Sprite()
{
}



#endif // !_SPRITE_
