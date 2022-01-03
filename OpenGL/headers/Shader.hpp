#pragma once

#ifndef SHADER_HPP
# define SHADER_HPP

# include <string>
# include <unordered_map>
# include <math/Matrix.hpp>
# include <GL/glew.h>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
private:
	std::string								m_FilePath;
	unsigned int							m_RendererID;
	std::unordered_map<std::string, int>	m_UniformLocationCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind()   const;
	void Unbind() const;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1iv(const std::string&name, int count, int *value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string &name, const Matrix<GLfloat, 4, 4> &m);
private:
	int GetUniformLocation(const std::string& name);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};

#endif
