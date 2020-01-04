
#ifndef _SENSOR_MANAGER
#define _SENSOR_MANAGER

#include "Anchor.h"
#include "Set.h"
#include "SensorManager.h"
#include "shared.h"

using namespace ci;
using namespace ci::app;
using namespace std;

/*
 * Virtual class Sensor.
 */
class Sensor {
public:
    Sensor(){};
    
    virtual bool setup() = 0;
    virtual void update(float aTimeInterval) = 0;
    virtual void draw() = 0;
    virtual void drawDebug(float x, float y, float w, float h){};
    virtual bool isReady(){return true;};
    
    virtual bool isCalibrated(){return true;};
    
    virtual ColorA getColorPixel(Vec2f aPosition) {return Color::black();};
    
    virtual void Calibrate(){};
    virtual void UpdateAnchors() = 0;
    Surface    mPixelsCalib;
    
};


/*
 * Wrapper and accessor methods to the sensor.
 */
class SensorManager {
public:
    SensorManager(){};
    
    /*
     * Creation of the singleton, return the instance.
     */
    static SensorManager * getInstance ();
    
    /*
     * setup the proper sensor.
     * will pick the best sensor available i.e kinect, then camera then mouse.
     * return false if none available.
     */
	bool setup();
    
    /*
     * Wrapper methods.
     */
    void Calibrate() {mSensor->Calibrate();};
    void draw() {mSensor->draw();};
    void drawDebug(float x, float y, float w, float h) {mSensor->drawDebug(x,y,w,h);};
    void update(float aTimeInterval) {mSensor->update(aTimeInterval);};
    bool isReady()  {return mSensor->isReady();};
    bool isCalibrated() {return mSensor->isCalibrated();};
    void UpdateAnchors(){mSensor->UpdateAnchors();};
    ColorA getColorPixel(Vec2f aPosition){return mSensor->getColorPixel(aPosition);};

private:
    
    Sensor * mSensor;
    static SensorManager* _SensorManagersingleton;
    
};

#endif
