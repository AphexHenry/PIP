
#include "KinectManager.h"

KinectManager::~KinectManager()
{
    mCalibCount = 0;
}

bool KinectManager::IsAvailable()
{
    return Kinect::getNumDevices() > 0;
}

bool KinectManager::isReady()
{
    return mKinect->checkNewDepthFrame();
}

bool KinectManager::setup()
{
	console() << "There are " << Kinect::getNumDevices() << " Kinects connected." << std::endl;
    
    mKinect = Kinect::create();
    mCalibrationDelay = 10.f;
    return mKinectWorking;
}

void KinectManager::update(float aTimeInterval)
{
	if( mKinect->checkNewDepthFrame() )
		mDepthSurface = mKinect->getDepthImage();
    
    if(mCalibCount > 0)
    {
        UpdateCalibrate(aTimeInterval);
    }
}

void KinectManager::UpdateAnchors()
{
    vector<vec3> lPositionsAll;
    
    float lDistanceCalib, lDistanceCurrent;
    
    if(!mDepthSurface || !mPixelsCalib)
    {
        return;
    }
    
    // we'll need to iterate the inputSurface as well as the output surface
    Surface::Iter inputIter( mDepthSurface.getIter() );
    Surface::Iter calibIter( mPixelsCalib.getIter(  ) );

    while( inputIter.line() && calibIter.line() ) {
        while( inputIter.pixel() && calibIter.pixel() ) {
            vec2 current = inputIter.getPos();
 
            lDistanceCalib = calibIter.r();
            lDistanceCurrent = inputIter.r();
            
            if(lDistanceCurrent > (lDistanceCalib * 1.4))
            {
                lPositionsAll.push_back(vec3(current.x / (float)mKinect->getWidth(),(float)current.y / (float)mKinect->getWidth(),lDistanceCalib * Set::coeffZKinect));
            }
        }
    }
    
    // draw anchors
    float gap = lPositionsAll.size() / Shared::sAnchors.size();
    for(int i = 0; (i < lPositionsAll.size()) && (i < Shared::sAnchors.size()); i++)
    {
        Shared::sAnchors[i]->SetPosition(lPositionsAll[floor((float)i * gap)]);
    }
}

void KinectManager::Calibrate()
{
    mCalibCount = 74;
}

void KinectManager::UpdateCalibrate(float aTimeInterval)
{
    if(!mDepthSurface && !mPixelsCalib)
    {
        return;
    }
    if(mDepthSurface && !mPixelsCalib)
    {
        mPixelsCalib = mDepthSurface;
    }
    
    mCalibrationDelay -= aTimeInterval;
    if(mCalibrationDelay < 0)
    {
        mCalibCount--;
        float lDistanceCalib, lDistanceCurrent;
        
        // we'll need to iterate the inputSurface as well as the output surface
        Surface::Iter inputIter( mDepthSurface.getIter() );
        Surface::Iter calibIter( mPixelsCalib.getIter(  ) );
        
        while( inputIter.line() && calibIter.line() ) {
            while( inputIter.pixel() && calibIter.pixel() ) {
                lDistanceCalib = calibIter.r();
                lDistanceCurrent = inputIter.r();
                calibIter.r() = max(lDistanceCalib, lDistanceCurrent);
//                if(lDistanceCalib > lDistanceCurrent)
//                {
//                    calibIter.r() -= 1;//lDistanceCurrent + (float)(lDistanceCalib - lDistanceCurrent) * 0.05f;
//                }
//                else
//                {
//                    calibIter.r() += 1;// = lDistanceCurrent + (lDistanceCalib - lDistanceCurrent) * 0.01f;
//                }
            }
        }
    }
}

void KinectManager::draw(){
    
    //    mBackground.draw(0, 0,Tools::getWindowWidth(), Tools::getWindowHeight());
    
    drawDebug(0,0,Tools::getWindowWidth(),Tools::getWindowHeight());
    
    Set::draw();
}

void KinectManager::drawDebug(float x, float y, float w, float h){

}
