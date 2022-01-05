#pragma once

#ifndef VERTEXBUFFER_HPP
# define VERTEXBUFFER_HPP

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	VertexBuffer(unsigned int size, const void* data);
	VertexBuffer(unsigned int size);
	~VertexBuffer();

	void Bind()   const;
	void Unbind() const;
private:
	VertexBuffer();
};

#endif
