#pragma once
#include <string>
#include <unordered_map>

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
	void SetUniform4f(const std::string& name, float v0, float v1, float f2, float f3); // should use math lib here -PC
	void SetUniform1f(const std::string& name, float value);

private:
	ShaderProgramSource ParseShader(const std::string& filepath);

	int GetUniformLocation(const std::string& name);

	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

	unsigned int CompileShader(unsigned int type, const std::string& source);
};

