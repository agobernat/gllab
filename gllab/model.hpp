#pragma once
#include "macros.hpp"
#include "tiny_gltf.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Model
{
public:
	Model();
	~Model();
	void bindModelNodes(std::map<int, GLuint> vbos, tinygltf::Model& model, tinygltf::Node& node);
	GLuint bindModel(tinygltf::Model& model);
	std::map<int, GLuint> bindMesh(std::map<int, GLuint> vbos,
		tinygltf::Model& model, tinygltf::Mesh& mesh);
	void drawMesh(tinygltf::Model& model, tinygltf::Mesh& mesh);
	void drawModelNodes(tinygltf::Model& model, tinygltf::Node& node);
	void drawModel(GLuint vao, tinygltf::Model& model);

private:

};

inline Model::Model()
{
}

inline Model::~Model()
{
}