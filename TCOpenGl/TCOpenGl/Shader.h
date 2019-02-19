#pragma once
#include <string>
#include <unordered_map>
#include "glm\glm.hpp"

struct ShaderProgramSource // struct used to end around multiple return types -PC
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocationCache;// caching system for uniforms
public:
	Shader(const std::string& filepath);
	virtual ~Shader();

	void Bind() const;
	void Unbind();

	// set uniforms
	void SetUniform1i(const std::string& name, int value); // this takes an int "value" for a sampler which is a int bound to a uniform which is passed to the GPU like a pointer to access the underling texture -PC
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3); // should use math lib here -PC
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
private:
	ShaderProgramSource ParseShader(const std::string& filepath);

	int GetUniformLocation(const std::string& name);

	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	unsigned int CompileShader(unsigned int type, const std::string& source);
};

