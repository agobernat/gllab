#pragma once
#include "tiny_gltf.h"

#define RGB_RGB 0
#define RGBA_RGB 1
#define RGB_RGBA 2
#define RGBA_RGBA 3



#define PI 3.14159265359

class Static
{
public:	
	static void init() {
		//loader = tinygltf::TinyGLTF();
	}
	static inline tinygltf::TinyGLTF loader;
	static inline size_t vaamax = 2;

	static inline unsigned int SCR_WIDTH = 1366;
	static inline unsigned int SCR_HEIGHT = 768;

private:

};