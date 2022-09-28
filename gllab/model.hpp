#pragma once

#include "tiny_gltf.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class GameModel
{
public:
	
	GameModel();
	~GameModel();
	bool loadFromFile(const std::string filename);
	struct PrimitiveData;
	std::map<int, PrimitiveData> calculatePrimitiveBufferParams(tinygltf::Mesh& mesh);

	void bindMesh(tinygltf::Mesh& mesh);

	void bindModelNodes(tinygltf::Node& node);

	void bind();

	void drawMesh(tinygltf::Mesh& mesh);

	void drawModelNodes(tinygltf::Node& node);

	void draw();

	void dbgModel();

	glm::mat4 genView(glm::vec3 pos, glm::vec3 lookat);

	glm::mat4 genMVP(glm::mat4 view_mat, glm::mat4 model_mat, float fov, int w,
		int h);

	//GLuint vao;
	


private:
	glm::mat4 model_mat;
	glm::mat4 model_rot;
	glm::vec3 model_pos;
	glm::mat4 view_mat;
	tinygltf::Model* modelData;

	struct BufferData
	{
		GLuint vbo;
		int bufferView;

		size_t accessorByteLength;
		size_t accessorByteOffset;
	};

	struct PrimitiveData
	{
		size_t currentBufferOffset;
		GLuint vao;
		std::map<int, BufferData> bufData;
	};



	std::map<int, PrimitiveData> meshData;

	
	
};

