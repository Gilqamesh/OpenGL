#ifndef ITEXTURE_HPP
# define ITEXTURE_HPP

class ITexture
{
unsigned int GL_ID;
public:
    virtual ~ITexture() = 0;
    inline void bind() const {}
    inline void unbind() const {}
};

#endif
