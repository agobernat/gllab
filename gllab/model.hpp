#pragma once

#pragma warning(push)
#pragma warning(suppress)



#include "tiny_gltf.h"

#pragma warning(pop)

#include <shader_t.h>
#include "camera.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class GameModel
{
private: struct PrimitiveData;
public:
	
	GameModel();
	~GameModel();
	bool loadFromFile(const std::string filename);
	struct PrimitiveData;
	std::map<int, PrimitiveData> calculatePrimitiveBufferParams(tinygltf::Mesh& mesh);

	void bindMesh(tinygltf::Mesh& mesh);

	void bindModelNodes(tinygltf::Node& node);

	void bind();

	void drawMesh(tinygltf::Mesh& mesh, const Camera& camera);

	void drawModelNodes(tinygltf::Node& node, const Camera& camera);

	void draw(const Camera& camera);

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
		const Shader* shader;
		size_t currentBufferOffset;
		GLuint vao;
		std::map<int, BufferData> bufData;
	};



	std::map<int, PrimitiveData> meshData;

	
	
};

