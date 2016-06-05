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
    
    vec3 mPositionMouse;
    
    list<vec3> mPositionsPart;
};

#endif
