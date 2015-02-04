
#ifndef _SENSOR_MANAGER
#define _SENSOR_MANAGER

#include "Anchor.h"
#include "Set.h"
#include "Subdivision.h"
#include "shared.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SensorManager {
public:
    SensorManager(){};
    
	virtual bool setup() = 0;
	virtual void update(float aTimeInterval) = 0;
	virtual void draw() = 0;
	virtual void drawDebug(float x, float y, float w, float h){};
    virtual bool isReady(){return true;};
    
	virtual void Calibrate(){};
	virtual void UpdateAnchors() = 0;
    Surface    mPixelsCalib;

};

#endif