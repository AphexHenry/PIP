#ifndef _CURSOR_MANAGER_H
#define _CURSOR_MANAGER_H

#include "cinder/Cinder.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"

#include "SensorManager.h"

class CursorManager : public SensorManager {
  public:
	bool setup();
	
	void update(float aTimeInterval);
	void draw();
    
    void UpdateAnchors();
    
    Vec3f mPositionMouse;
    
    list<Vec3f> mPositionsPart;
};

#endif
