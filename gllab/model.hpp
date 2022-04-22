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
	bool loadModel(tinygltf::Model& model, const char* filename);

	std::map<int, GLuint> bindMesh(std::map<int, GLuint> vbos,
		tinygltf::Model& model, tinygltf::Mesh& mesh);

	void bindModelNodes(std::map<int, GLuint> vbos, tinygltf::Model& model,
		tinygltf::Node& node);

	GLuint bindModel(tinygltf::Model& model);

	void drawMesh(tinygltf::Model& model, tinygltf::Mesh& mesh);

	void drawModelNodes(tinygltf::Model& model, tinygltf::Node& node);

	void drawModel(GLuint vao, tinygltf::Model& model);

	void dbgModel(tinygltf::Model& model);

	glm::mat4 genView(glm::vec3 pos, glm::vec3 lookat);

	glm::mat4 genMVP(glm::mat4 view_mat, glm::mat4 model_mat, float fov, int w,
		int h);

	GLuint vao;

	glm::mat4 model_mat;
	glm::mat4 model_rot;
	glm::vec3 model_pos;
	glm::mat4 view_mat;

private:
	
};
