#ifndef RENDERER_HPP
# define RENDERER_HPP

# include <GL/glew.h>
# include "VertexArray.hpp"
# include "IndexBuffer.hpp"
# include "Shader.hpp"
# include <csignal>
# include "Trace.hpp"

# define ASSERT(x) if(!(x)) raise(SIGTRAP);
# define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError(void);
bool GLLogCall(const char* function, const char* file, int line);
# ifndef LOG
#  define LOG(x) (std::cout << x << std::endl)
# endif
// # define TRACE(x) (Trace::add(x))
# define TRACE() (Trace::add("File: " + std::string(__FILE__).substr(std::string(__FILE__).find_last_of("/") + 1) + ", \t\tfunction: " \
    + std::string(__FUNCTION__) + ", \t\tline: " + std::to_string(__LINE__)))

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Clear() const;
    void Draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
};

#endif
