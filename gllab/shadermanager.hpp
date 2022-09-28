#pragma once
#include <unordered_map>
#include "shader_t.h"

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	ShaderManager(const ShaderManager&) = delete;
	ShaderManager(ShaderManager&&) = delete;
	ShaderManager& operator=(const ShaderManager&) = delete;
	ShaderManager& operator=(ShaderManager&&) = delete;

	const Shader& getShader(const char* name);

	const Shader& getShaderByMaterial(const char* material);

	void setMaterial(const Shader& shader, const char* material);
	void setMaterial(const char* name, const char* material);

	void setDefaultShader(const char* name);
	void setDefaultShader(const Shader& shader);
	

	void loadShader(const char* name, const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr,
		const char* tessControlPath = nullptr, const char* tessEvalPath = nullptr);

private:
	std::string defaultName;
	std::unordered_map<std::string, Shader*> shaders;
	std::unordered_map<std::string, std::string> materials;

};

