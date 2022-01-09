#pragma once
#ifndef TEXTURE_HPP
# define TEXTURE_HPP

# include "Renderer.hpp"
# include <string>
# include "math/Matrix.hpp"

class Texture
{
private:
	enum class type
	{
		COLOR,
		TEX
	};

	unsigned int		m_RendererID;
	std::string			m_FilePath;
	int					m_Width;
	int					m_Height;
	int					m_BPP;
	Vector<float, 4>	m_Color;
	type				m_type;

public:
	Texture() = default;
	Texture(const std::string& path);
	Texture(const char *path);
	Texture(const Vector<float, 4>& color);
	Texture(const Texture&);
	Texture& operator=(const Texture&);
	~Texture();

	void Bind(unsigned int slot = 0)   const;
	void Unbind() const;

	inline bool isLoadedFromFile(void) const { return (m_type == type::TEX); }

	inline int GetWidth()  const { return (m_Width); }
	inline int GetHeight() const { return (m_Height); }

	inline const Vector<float, 4>& getColor(void) const { return (m_Color); }
};

#endif
