#pragma once

#ifndef SHADER_HPP
# define SHADER_HPP

# include <string>
# include <unordered_map>
# include <math/Matrix.hpp>
# include <GL/glew.h>
# include "MaterialTypes.hpp"
# include "LightSource.hpp"

class Shader
{
private:
	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;
	};
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string								m_FilePath;
	unsigned int							m_RendererID;
	std::unordered_map<std::string, int>	m_UniformLocationCache;
	std::string								shaderName;
	std::string								m_VertexShaderPath;
	std::string								m_FragmentShaderPath;
public:
	Shader(const std::string& filepath);
	Shader(const std::string& filepath, const std::string& name);
	Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& name);
	~Shader();

	void Bind()   const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1iv(const std::string&name, int count, int *value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform3f(const std::string& name, float f1, float f2, float f3);
	void SetUniform3f(const std::string& name, const Vector<float, 3> vec3);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform4f(const std::string& name, const Vector<float, 4> vec4);
	void SetUniformMat4fv(const std::string &name, int count, const Matrix<float, 4, 4> &m);
	void SetUniformMaterial(const Material &m);
	void SetUniformLightSource(const LightSource &l);
	void SetUniformLightSource(const LightSource &l, int id);
private:
	int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShaders(const std::string& filepath);
	std::string ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShaders(const std::string& vertexShader, const std::string& fragmentShader);
	unsigned int CreateShader(const std::string& shaderCode, GLuint shaderType);
};

#endif
