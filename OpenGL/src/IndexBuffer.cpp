#include "IndexBuffer.hpp"
#include "Renderer.hpp"

IndexBuffer::IndexBuffer()
{

}

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
    : m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GLCall(glGenBuffers(1, &m_RendererID));
    // select created buffer
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
    // specify data, how big, what data and how often
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(const IndexBuffer& i)
    : m_Count(i.m_Count), m_RendererID(i.m_RendererID)
{

}

IndexBuffer& IndexBuffer::operator=(const IndexBuffer& i)
{
    if (this != &i)
    {
        m_Count = i.m_Count;
        m_RendererID = i.m_RendererID;
    }
    return (*this);
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
