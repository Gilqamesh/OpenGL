#ifndef IMATERIAL_HPP
# define IMATERIAL_HPP

# include "Interfaces/ITexture.hpp"

class IMaterial
{
ITexture *ambient;
ITexture *diffuse;
ITexture *specular;
ITexture *emission;
float shininess;
public:
    virtual ~IMaterial() = 0;
    inline const ITexture *getAmbient() const { return (ambient); }
    inline const ITexture *getDiffuse() const { return (diffuse); }
    inline const ITexture *getSpecular() const { return (specular); }
    inline const ITexture *getEmission() const { return (emission); }
    inline float getShininess() const { return (shininess); }

    inline void setAmbient(ITexture *ambient) { this->ambient = ambient; }
    inline void setDiffuse(ITexture *diffuse) { this->diffuse = diffuse; }
    inline void setSpecular(ITexture *specular) { this->specular = specular; }
    inline void setEmission(ITexture *emission) { this->emission = emission; }

    
};

#endif
