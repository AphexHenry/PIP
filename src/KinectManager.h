#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/Surface.h"
#include "cinder/ImageIo.h"
#include "cinder/Utilities.h"
#include "cinder/Rand.h"

#include "CinderFreenect.h"

#include "SensorManager.h"

using namespace ci;
using namespace ci::app;
using namespace std;

#define NUM_USER 1
#define DISTANCE_MAX 10000.

class KinectManager : public SensorManager {
public:
    ~KinectManager();
    
    static bool IsAvailable();
	bool setup();
	void update(float aTimeInterval);
	void draw();
    bool isReady();
	
	KinectRef		mKinect;
    ImageSourceRef mDepthSurface;
    
    void drawDebug(float x, float y, float w, float h);
    void exit();
    void Calibrate();
    void UpdateCalibrate(float aTimeInterval);
    void UpdateAnchors();
    void UpdateOSC();
    
    float mCalibrationDelay;
    
private:
    
    int   mCalibCount;
    float mLastTick;
    vec2 mMousePos;
    
    bool  mKinectWorking;
};
