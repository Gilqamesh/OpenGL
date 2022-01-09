#include "VertexBuffer.hpp"
#include "Renderer.hpp"
#include <array>

VertexBuffer::VertexBuffer()
    : m_RendererID(1000000)
{

}

VertexBuffer::VertexBuffer(unsigned int size, const void* data)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(unsigned int size)
{
    GLCall(glGenBuffers(1, &m_RendererID));
    // select created buffer
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
    // specify data, how big, what data and how often
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

VertexBuffer::VertexBuffer(const VertexBuffer& v)
    : m_RendererID(v.m_RendererID)
{

}

VertexBuffer& VertexBuffer::operator=(const VertexBuffer& v)
{
    if (this != &v)
    {
        m_RendererID = v.m_RendererID;
    }
    return (*this);
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
