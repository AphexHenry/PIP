#ifndef _CAPTURE_MANAGER_H
#define _CAPTURE_MANAGER_H

#include "cinder/Cinder.h"
#include "cinder/app/AppNative.h"
#include "cinder/gl/Texture.h"
#include "cinder/Capture.h"

#include "SensorManager.h"

using namespace ci;
using namespace ci::app;

class IntroManager {
  public:
	void setup();
	void update(float aTimeInterval);
	void draw();
    void runApp();
};
#endif