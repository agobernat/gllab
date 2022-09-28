#include "shadermanager.hpp"


ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
	for (auto& shader : shaders) {
		delete shader.second;
	}
}

const Shader& ShaderManager::getShader(const char* name) {
	if (!shaders.contains(name))
	{
		throw std::invalid_argument("Shader does not exist");
	}
	return *shaders[name];
}

const Shader& ShaderManager::getShaderByMaterial(const char* material) {

	if (!materials.contains(material))
	{
		if (defaultName.size() > 0)
		{
			return *shaders[defaultName];
		}
		throw std::invalid_argument("Material does not exist");
	}
	return *shaders[materials[material]];
}

void ShaderManager::setDefaultShader(const Shader& shader) {
	setDefaultShader(shader.name);
}

void ShaderManager::setDefaultShader(const char* name) {
	if (!shaders.contains(name))
	{
		throw std::invalid_argument("Shader does not exist");
	}
	defaultName = name;
}


void ShaderManager::setMaterial(const Shader& shader, const char* material) 
{
	setMaterial(shader.name, material);
}

void ShaderManager::setMaterial(const char* name, const char* material) {
	if (!shaders.contains(name))
	{
		throw std::invalid_argument("Shader does not exist");
	}
	if (materials.contains(name))
	{
		throw std::invalid_argument("Material is already defined for this shader");
	}
	else
	{
		materials[material] = name;
	}
}

void ShaderManager::loadShader(const char* name, const char* vertexPath, const char* fragmentPath, const char* geometryPath,
	const char* tessControlPath, const char* tessEvalPath) 
{
	if (shaders.contains(name))
	{
		throw std::invalid_argument("Shader already defined");
	}

	Shader* shader = new Shader(vertexPath, fragmentPath, geometryPath, tessControlPath, tessEvalPath);
	shader->name = name;
	shaders[name] = shader;

}