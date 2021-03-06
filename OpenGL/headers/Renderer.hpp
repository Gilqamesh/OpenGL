#pragma once
#ifndef RENDERER_HPP
# define RENDERER_HPP

# include <GL/glew.h>
# include "VertexArray.hpp"
# include "IndexBuffer.hpp"
# include "Shader.hpp"

# define ASSERT(x) if(!(x)) __debugbreak();
# define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(void);
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Clear() const;
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
};

#endif
