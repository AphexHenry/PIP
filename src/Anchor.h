#ifndef _ANCHOR_H
#define _ANCHOR_H

#include "Math.h"
#include "Tools.h"
#include "cinder/cinder.h"
#include "cinder/Vector.h"
#include "cinder/Color.h"

/*
 *  Anchor define a point in the space.
 *  It is the base element for particles and shape capture.
 */
class Anchor{

public:
    
    /*
     *  Constructor.
     */
    Anchor();
    
    /*
     *  draw the point (debug purpose).
     */
    void draw();
    
    /*
     *  set the position of the anchor.
     */
    void SetPosition(ci::Vec3f aPosition){mPosition = aPosition;};
    
    /*
     *  anchor datas.
     */
    ci::Vec3f mPosition;
    float   mSize;
    ci::ColorA   mColor;
};

#endif
