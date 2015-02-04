#ifndef _ANCHOR_H
#define _ANCHOR_H

#include "Math.h"
#include "Tools.h"
#include "cinder/cinder.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"

class Anchor{

public:
    
    Anchor(int aUser);
    void update();
    void drawDebug();
    void SetPosition(ci::Vec3f aPosition){mPosition = aPosition;};
    ci::Vec3f mPosition;
    float   mSize;
    ci::Color   mColor;
    
private:
    int     mUser;
};

#endif
