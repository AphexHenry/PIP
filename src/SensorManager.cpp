
#include "SensorManager.h"
#include "KinectManager.h"
#include "CaptureManager.h"
#include "CursorManager.h"

SensorManager *SensorManager::_SensorManagersingleton = NULL;

/*
 * Creation of the singleton, return the instance.
 */
SensorManager * SensorManager::getInstance ()
{
    if (NULL == _SensorManagersingleton)
    {
        _SensorManagersingleton =  new SensorManager;
    }
    
    return _SensorManagersingleton;
}

bool SensorManager::setup()
{   
    if(KinectManager::IsAvailable())
    {
        mSensor = new KinectManager();
    }
    else if(CaptureManager::IsAvailable())
    {
        // if no kinect is connected, use video.
        mSensor = new CaptureManager();
    }
    else
    {
        mSensor = new CursorManager();
    }

    mSensor->setup();
}