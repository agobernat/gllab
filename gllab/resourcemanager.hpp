#pragma once
#include "include/glm/glm.hpp"


class ResourceManager 
{
public:
	
	static unsigned int LoadTexture(const char* filename, int dest_color, int src_color, int scaling_min, int scaling_max);
	static unsigned int MakeTextureFromColor(glm::vec4 color);



private:
	ResourceManager();
	
};

