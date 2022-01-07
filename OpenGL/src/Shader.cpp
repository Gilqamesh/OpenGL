#include "Shader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Renderer.hpp"

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
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

ShaderProgramSource Shader::ParseShader(const std::string &filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        // c-type allocation I think? Idk why are we using it here instead of new then delete
        //char* message = (char*)alloca(length * sizeof(char));
        char* message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile "
            << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        delete message;
        glDeleteShader(id);
        return (0);
    }

    return (id);
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

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
    GLCall(glUniformMatrix4fv(GetUniformLocation(name), count, GL_FALSE, &m));
}

void Shader::SetUniformMaterial(const Material& m)
{
    this->SetUniform1i("material.colorType", m.getColorType());
    if (m.getColorType() == static_cast<int>(Material::colorType::TEX))
    {
        this->SetUniform1i("material.color.lightingMaps.diffuseMap", m.getDiffuseMap());
        this->SetUniform1i("material.color.lightingMaps.specularMap", m.getSpecularMap());
        this->SetUniform1i("material.color.lightingMaps.emissionMap", m.getEmissionMap());
    }
    else if (m.getColorType() == static_cast<int>(Material::colorType::COLOR))
    {
        this->SetUniform3f("material.color.color.ambientColor", m.getAmbientColor());
        this->SetUniform3f("material.color.color.diffuseColor", m.getDiffuseColor());
        this->SetUniform3f("material.color.color.specularColor", m.getSpecularColor());
    }
    this->SetUniform1f("material.specularStrength", m.getSpecularFactor());
    this->SetUniform1f("material.shininessFactor", m.getShininessFactor());
}

void Shader::SetUniformLightSource(const LightSource& l)
{
    this->SetUniform1i("light.type", l.getType());
    this->SetUniform3f("light.position", l.getPosition());
    this->SetUniform3f("light.ambientColor", l.getAmbientColor());
    this->SetUniform3f("light.diffuseColor", l.getDiffuseColor());
    this->SetUniform3f("light.specularColor", l.getSpecularColor());
    this->SetUniform1f("light.attenuation.constant", l.getAttenuation_Constant());
    this->SetUniform1f("light.attenuation.linear", l.getAttenuation_Linear());
    this->SetUniform1f("light.attenuation.quadratic", l.getAttenuation_Quadratic());
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return (m_UniformLocationCache[name]);

    GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
        std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
    
    m_UniformLocationCache[name] = location;
    return (location);
}
