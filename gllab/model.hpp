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
#include "gameobject.hpp"
#include "include/bullet/btBulletDynamicsCommon.h"



struct Transform;
class GameModel
{
private: struct PrimitiveData;
public:
	
	GameModel();
	~GameModel();
	bool loadFromFile(const std::string filename);
	struct PrimitiveData;
	std::map<int, PrimitiveData> calculatePrimitiveBufferParams(tinygltf::Mesh& mesh);

	glm::mat4 calculateModelMat(const tinygltf::Node& node, const Transform& transform) const;

	void setColliderFromMesh();

	void setCustomCollider(btVector3 dimensions);

	btCollisionShape* getCollisionShape() const;

	void bindMesh(tinygltf::Mesh& mesh, const tinygltf::Node& node);

	void bindModelNodes(tinygltf::Node& node);

	void bind();

	void drawMesh(tinygltf::Mesh& mesh, const Camera& camera, const tinygltf::Node& node, const Transform& transform) const;

	void drawModelNodes(tinygltf::Node& node, const Camera& camera, const Transform& transform) const;

	void draw(const Camera& camera) const;

	void draw(const Camera& camera, const Transform& transform) const;

	void dbgModel();

	glm::mat4 genView(glm::vec3 pos, glm::vec3 lookat);

	glm::mat4 genMVP(glm::mat4 view_mat, glm::mat4 model_mat, float fov, int w,
		int h);
	//GLuint vao;
	double getScalingFactorFromAccessors() const;


private:
	glm::mat4 model_mat;
	glm::mat4 model_rot;
	glm::vec3 model_pos;
	glm::mat4 view_mat;
	std::unique_ptr<tinygltf::Model> modelData;
	std::unique_ptr<btCollisionShape> collisionShape;

	struct SceneData {
		
	};

	struct BufferData
	{
		GLuint vbo;
		int bufferView;

		size_t accessorByteLength;
		size_t accessorByteOffset;
	};

	struct PrimitiveData
	{

		GLuint texture;
		const Shader* shader;
		size_t currentBufferOffset;
		GLuint vao;
		std::map<int, BufferData> bufData;
	};



	std::map<int, PrimitiveData> meshData;

	
	
};

