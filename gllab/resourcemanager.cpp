#include "resourcemanager.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string> 
//#include <learnopengl/filesystem.h>
//#include "macros.hpp"
#include <filesystem>

#include "tiny_gltf.h"
#include "stb_image.h"
//#include "macros.hpp"

	
unsigned int ResourceManager::LoadTexture(const char* filename, int dest_color, int src_color, int scaling_min, int scaling_max)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, scaling_min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, scaling_max);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	

	std::string rootdir = std::filesystem::current_path().string();

	rootdir.append("\\");
	rootdir.append(filename);

	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char *data = stbi_load(rootdir.c_str() , &width, &height, &nrChannels, 0);
	if (data)
	{
		
		glTexImage2D(GL_TEXTURE_2D, 0, dest_color, width, height, 0, src_color, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture: " << rootdir << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

unsigned int ResourceManager::MakeTextureFromColor(glm::vec4 color)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;

	float clr[4] = { color.x, color.y, color.z, color.w };


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_FLOAT, clr);
	glGenerateMipmap(GL_TEXTURE_2D);

	return texture;
}
