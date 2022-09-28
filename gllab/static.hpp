#pragma once
#include "tiny_gltf.h"

class Static
{
public:	
	static void init() {
		//loader = tinygltf::TinyGLTF();
	}
	static inline tinygltf::TinyGLTF loader;
	static inline size_t vaamax = 2;

private:

};