#ifndef _PARTICLE_BILLBOARD_TREE_H
#define _PARTICLE_BILLBOARD_TREE_H

#include "Particle.h"
#include "cinder/gl/Texture.h"

class ParticleBillboardTree : public Particle{

public:
    
    ParticleBillboardTree():Particle(){mTextureIndex = 0;};
    
    /*
     *  draw for debug purpose.
     */
    void drawDebug(int aCoeffLifeColor);
    
    /*
     *  update.
     */
    void update(float aTimeInterval);
    
    void Reset();
    
    
    static void AddTexture(ci::gl::Texture aTexture);
    
private:
    
    float   mAlpha;
    float   mRotation;
    float   mRotationSpeed;
    int     mTextureIndex;
    static std::vector< ci::gl::Texture > sTextures;
};

#endif
