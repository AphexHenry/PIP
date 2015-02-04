#ifndef _PARTICLE_NIV_1_H
#define _PARTICLE_NIV_1_H

#include "Math.h"
#include "Anchor.h"
#include "cinder/Color.h"
#include <vector>

class Particle : public Anchor{

public:
    
    Particle();
    virtual void update(float aTimeInterval);

    /*
     *  put the particles in the grid.
     */
    void updateGrid();
    
    /*
     *  draw for debug purpose.
     */
    virtual void draw(int aCoeffLifeColor);
    
    /*
     *  Get a close target.
     */
    Anchor * GetTarget();
    
    /*
     *  If true, will draw upside down, like a reflection.
     */
    static void SetReflection (bool aReflection) {sReflection = aReflection;};
    
private:
    bool CheckY(int y);
    bool CheckX(int x);
    
    ci::Vec3f mSpeed;
    ci::Vec3f mPositionLast;
    
    float mImmuneTimer;
    std::vector< std::vector< Anchor * > > * mSubdivisions;

protected:
    virtual void Reset();
    ci::Vec3f GetPositionScene();
    
    static bool    sReflection;
    float     mLifeTime, mLifeTimeInit;
};

#endif
