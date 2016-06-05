#ifndef _ANCHOR_H
#define _ANCHOR_H

#include "Math.h"
#include "Tools.h"
#include "cinder/cinder.h"
#include "cinder/Vector.h"

class Anchor{

public:
    
    Anchor(int aUser);
    void update();
    void drawDebug();
    void SetPosition(ci::vec3 aPosition){mPosition = aPosition;};
    ci::vec3 mPosition;
    float   mSize;
    
private:
    int     mUser;
};

#endif
