#include "VertexArray.hpp"
#include "VertexBufferLayout.hpp"
#include "Renderer.hpp"

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
    TRACE();
    Bind();
	vb.Bind();
    const auto& elements = layout.GetElements();

    for (unsigned int i = 0, offset = 0; i < elements.size(); ++i)
    {
        const auto& element = elements[i];
        // we need to enable each attribute in the vertex
        GLCall(glEnableVertexAttribArray(i));
        // further specify the data, how does the data layout look like
        GLCall(glVertexAttribPointer(i, element.count, element.type,
            element.normalized, layout.GetStride(), reinterpret_cast<const void *>(offset)));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}

void VertexArray::Bind() const
{
    TRACE();
    GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    TRACE();
    GLCall(glBindVertexArray(0));
}
