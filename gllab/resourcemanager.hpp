#ifndef _RESOURCE_MANAGER_
#define _RESOURCE_MANAGER_



class ResourceManager 
{
public:
	
	static unsigned int LoadTexture(const char* filename, int dest_color, int src_color, int scaling_min, int scaling_max);




private:
	ResourceManager();
	
};





#endif // !_RESOURCE_MANAGER_
