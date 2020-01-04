#ifndef _CAPTURE_MANAGER_H
#define _CAPTURE_MANAGER_H

#include "cinder/Cinder.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"

#include "SensorManager.h"

using namespace ci;
using namespace ci::app;

/*
 *  This sensor is the camera of your computer.
 *  It will process the movements of the video and put the anchors to the moving pixels.
 */
class CaptureManager : public Sensor {
  public:
    
    /*
     *  setup the computer camera.
     *  return false if none can be found.
     */
	bool setup();
    
    /*
     *  return true if camera is available, false otherwise.
     */
    static bool IsAvailable();
	
    /*
     *  update the capture image.
     */
	void update(float aTimeInterval);
    
    /*
     *  draw the camera image.
     *  debug purpose.
     */
	void draw();
    
    /*
     *  return true when the capture can start.
     */
    bool isReady();
    
    /*
     *  start calibration.
     *  in case of this camera, the calibration is done every frame.
     */
    void Calibrate();
    
    /*
     *  update the anchors positions.
     */
    void UpdateAnchors();

private:
    /*
     *  reference to the cinder capture manager.
     */
	CaptureRef			mCapture;
    
    /*
     *  last captured frame.
     */
    Surface             mSurface;
    
    /*
     *  average of the luminosity.
     *  used to adapt the sensibility.
     */
    float               mAmbientLuminosity;
};
#endif