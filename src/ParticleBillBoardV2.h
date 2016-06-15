#ifndef _PARTICLE_BILLBOARD_V2_H
#define _PARTICLE_BILLBOARD_V2_H

#include "Particle.h"
#include "cinder/gl/Texture.h"

class ParticleBillboardV2 : public Particle{

public:
    
    ParticleBillboardV2():Particle(){mTextureIndex = 0;};
    
    /*
     *  draw for debug purpose.
     */
    void draw(bool aIsLeft) override;
    
    /*
     *  update.
     */
    void update(float aTimeInterval) override;
    
    /*
     *  Reset particle state.
     */
    void Reset() override;
    
    static void AddTexture(ci::gl::Texture aTexture);
    
private:
    
    float   mSizeInit;
    float   mAlpha;
    float   mRotation;
    float   mRotationSpeed;
    int     mTextureIndex;
    static std::vector< ci::gl::Texture > sTextures;
};

#endif
