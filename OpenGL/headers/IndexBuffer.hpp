#pragma once

#ifndef INDEXBUFFER_HPP
# define INDEXBUFFER_HPP

class IndexBuffer
{
private:
	unsigned int m_Count;
	unsigned int m_RendererID;
public:
	IndexBuffer(const unsigned int *data, unsigned int count);
	~IndexBuffer();

	void Bind()   const;
	void Unbind() const;

	inline unsigned int GetCount() const { return (m_Count); }
};

#endif
