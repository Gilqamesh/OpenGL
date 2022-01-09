#pragma once

#ifndef VERTEXBUFFER_HPP
# define VERTEXBUFFER_HPP

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer();
	VertexBuffer(unsigned int size, const void* data);
	VertexBuffer(unsigned int size);
	VertexBuffer(const VertexBuffer&);
	VertexBuffer& operator=(const VertexBuffer&);
	~VertexBuffer();

	void Bind()   const;
	void Unbind() const;
};

#endif
