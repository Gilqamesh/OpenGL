#include "Shader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.hpp"

Shader::Shader()
{

}

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShaders(filepath);
    m_RendererID = CreateShaders(source.VertexSource, source.FragmentSource);
}

Shader::Shader(const std::string& filepath, const std::string& name)
    : m_FilePath(filepath), shaderName(name)
{
    ShaderProgramSource source = ParseShaders(filepath);
    m_RendererID = CreateShaders(source.VertexSource, source.FragmentSource);
}

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath, const std::string& name)
    : m_VertexShaderPath(vertexShaderPath), m_FragmentShaderPath(fragmentShaderPath), shaderName(name)
{
    std::string vertexShaderCode = ParseShader(m_VertexShaderPath);
    std::string fragmentSaderCode = ParseShader(m_FragmentShaderPath);

    m_RendererID = CreateShaders(vertexShaderCode, fragmentSaderCode);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

Shader::ShaderProgramSource Shader::ParseShaders(const std::string &filepath)
{
    std::ifstream stream(filepath);

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            if (type == ShaderType::NONE)
                throw std::runtime_error("Didn't find shader type in " + std::string(__FUNCTION__));
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

std::string Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    if (!stream)
        throw std::runtime_error("Could not open shader file for parsing: " + filepath);

    std::string line;
    std::stringstream ss;
    while (getline(stream, line))
        ss << line << "\n";
    return (ss.str());
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    GLCall(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        // c-type allocation I think? Idk why are we using it here instead of new then delete
        //char* message = (char*)alloca(length * sizeof(char));
        char* message = new char[length];
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile shader! " << shaderName << std::endl;
        std::cout << message << std::endl;
        delete [] message;
        GLCall(glDeleteShader(id));
        return (0);
    }

    return (id);
}

unsigned int Shader::CreateShaders(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return (program);
}

unsigned int Shader::CreateShader(const std::string& shaderCode, GLuint shaderType)
{
    unsigned int program = glCreateProgram();
    unsigned int shaderId = CompileShader(shaderType, shaderCode);

    glAttachShader(program, shaderId);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(shaderId);

    return (program);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1iv(const std::string& name, int count, int* value)
{
    GLCall(glUniform1iv(GetUniformLocation(name), count, value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform3f(const std::string& name, float f1, float f2, float f3)
{
    GLCall(glUniform3f(GetUniformLocation(name), f1, f2, f3));
}

void Shader::SetUniform3f(const std::string& name, const Vector<float, 3> vec3)
{
    GLCall(glUniform3f(GetUniformLocation(name), vec3[0], vec3[1], vec3[2]));
}

void Shader::SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniform4f(const std::string& name, const Vector<float, 4> vec4)
{
    GLCall(glUniform4f(GetUniformLocation(name), vec4[0], vec4[1], vec4[2], vec4[3]));
}

void Shader::SetUniformMat4fv(const std::string &name, int count, const Matrix<GLfloat, 4, 4> &m)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), count, GL_FALSE, m.data()));
}

void Shader::SetUniformLightSource(const LightSource& l)
{
    this->SetUniform1i("light.type", l.getType());
    this->SetUniform3f("light.position", l.getPosition());
    this->SetUniform4f("light.color", l.getColor());
    this->SetUniform3f("light.direction", l.getDirection());
    this->SetUniform1f("light.innerCutOffAngle", cos(l.getInnerCutOffAngle()));
    this->SetUniform1f("light.outerCutOffAngle", cos(l.getOuterCutOffAngle()));
    this->SetUniform1f("light.ambientFactor", l.getAmbientFactor());
    this->SetUniform1f("light.diffuseFactor", l.getDiffuseFactor());
    this->SetUniform1f("light.specularFactor", l.getSpecularFactor());
    this->SetUniform1f("light.attenuation_factor_constant", l.getAttenuation_Constant());
    this->SetUniform1f("light.attenuation_factor_linear", l.getAttenuation_Linear());
    this->SetUniform1f("light.attenuation_factor_quadratic", l.getAttenuation_Quadratic());
}

void Shader::SetUniformLightSource(const LightSource& l, int id)
{
    this->SetUniform1i("light[" + std::to_string(id) + "].type", l.getType());
    this->SetUniform3f("light[" + std::to_string(id) + "].position", l.getPosition());
    this->SetUniform4f("light[" + std::to_string(id) + "].color", l.getColor());
    this->SetUniform3f("light[" + std::to_string(id) + "].direction", l.getDirection());
    this->SetUniform1f("light[" + std::to_string(id) + "].innerCutOffAngle", cos(l.getInnerCutOffAngle()));
    this->SetUniform1f("light[" + std::to_string(id) + "].outerCutOffAngle", cos(l.getOuterCutOffAngle()));
    this->SetUniform1f("light[" + std::to_string(id) + "].ambientFactor", l.getAmbientFactor());
    this->SetUniform1f("light[" + std::to_string(id) + "].diffuseFactor", l.getDiffuseFactor());
    this->SetUniform1f("light[" + std::to_string(id) + "].specularFactor", l.getSpecularFactor());
    this->SetUniform1f("light[" + std::to_string(id) + "].attenuation_factor_constant", l.getAttenuation_Constant());
    this->SetUniform1f("light[" + std::to_string(id) + "].attenuation_factor_linear", l.getAttenuation_Linear());
    this->SetUniform1f("light[" + std::to_string(id) + "].attenuation_factor_quadratic", l.getAttenuation_Quadratic());
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return (m_UniformLocationCache[name]);

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist! " << shaderName << std::endl;
    
    m_UniformLocationCache[name] = location;
    return (location);
}
