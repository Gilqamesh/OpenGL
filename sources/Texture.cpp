#include "Texture.hpp"

#include "vendor/stb_image/stb_image.h"

Texture::Texture(const std::string& path)
	: m_FilePath(path), m_Width(0), m_Height(0), m_BPP(0), m_type(type::TEX)
{
    TRACE();
	unsigned char* buffer;

	stbi_set_flip_vertically_on_load(1);
	buffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// Need to specify these 4 parameters
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));
	
	// Generate mipmap
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (buffer)
		stbi_image_free(buffer);
}

Texture::Texture(const char* path)
	: m_FilePath(path), m_Width(0), m_Height(0), m_BPP(0), m_type(type::TEX)
{
    TRACE();
	unsigned char* buffer;

	stbi_set_flip_vertically_on_load(1);
	buffer = stbi_load(path, &m_Width, &m_Height, &m_BPP, 4);

	GLCall(glGenTextures(1, &m_RendererID));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	// Need to specify these 4 parameters
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer));

	// Generate mipmap
	GLCall(glGenerateMipmap(GL_TEXTURE_2D));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (buffer)
		stbi_image_free(buffer);
}

Texture::Texture(const Vector<float, 4>& color)
	: m_Color(color), m_type(type::COLOR)
{

}

Texture::Texture(const Texture& t)
	: m_RendererID(t.m_RendererID), m_FilePath(t.m_FilePath), m_Width(t.m_Width), m_Height(t.m_Height), m_BPP(t.m_BPP), m_Color(t.m_Color), m_type(t.m_type)
{

}

Texture& Texture::operator=(const Texture& t)
{
	if (this != &t)
	{
		m_RendererID = t.m_RendererID;
		m_Width = t.m_Width;
		m_Height = t.m_Height;
		m_BPP = t.m_BPP;
		m_Color = t.m_Color;
		m_type = t.m_type;
	}
	return (*this);
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
    TRACE();
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
    TRACE();
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
