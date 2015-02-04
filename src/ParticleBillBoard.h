#ifndef _PARTICLE_BILLBOARD_H
#define _PARTICLE_BILLBOARD_H

#include "Particle.h"
#include "cinder/gl/Texture.h"

class ParticleBillboard : public Particle{

public:
    
    ParticleBillboard():Particle(){};
    
    /*
     *  draw particle.
     */
    void draw(int aCoeffLifeColor);
    
    static void SetTexture(ci::gl::Texture aTexture);
    
private:

    static ci::gl::Texture sTexture;
};

#endif
