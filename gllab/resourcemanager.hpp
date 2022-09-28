#pragma once



class ResourceManager 
{
public:
	
	static unsigned int LoadTexture(const char* filename, int dest_color, int src_color, int scaling_min, int scaling_max);




private:
	ResourceManager();
	
};

