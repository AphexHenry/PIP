
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
    
    if( mKinect->checkNewVideoFrame() )
		mVideoSurface = mKinect->getVideoImage();
    
    if(mCalibCount > 0)
    {
        UpdateCalibrate(aTimeInterval);
    }
}

void KinectManager::UpdateAnchors()
{
    vector<Vec3f> lPositionsAll;
    vector<Color> lColorsAll;
    
    float lDistanceCalib, lDistanceCurrent;
    
    if(!mDepthSurface || !mPixelsCalib || !mVideoSurface)
    {
        return;
    }
    
    // we'll need to iterate the inputSurface as well as the output surface
    Surface::Iter inputIter( mDepthSurface.getIter() );
    Surface::Iter videoIter( mVideoSurface.getIter(  ) );
    Surface::Iter calibIter( mPixelsCalib.getIter(  ) );

    while( inputIter.line() && calibIter.line() && videoIter.line() ) {
        while( inputIter.pixel() && calibIter.pixel() && videoIter.pixel() ) {
            Vec2f current = inputIter.getPos();
 
            lDistanceCalib = calibIter.r();
            lDistanceCurrent = inputIter.r();
            if(lDistanceCurrent >= 254)
                lDistanceCurrent = 0;
            
            if(lDistanceCurrent > (lDistanceCalib * 1.4))
            {
                lPositionsAll.push_back(Vec3f(current.x / (float)mKinect->getWidth(),(float)current.y / (float)mKinect->getWidth(),lDistanceCalib * Set::coeffZKinect));
                lColorsAll.push_back(Color((float)videoIter.r() / 255.f, (float)videoIter.g() / 255.f, (float)videoIter.b() / 255.f));
            }
        }
    }
    
    float lZ = 0.f;
    for(int i = 0; i < lPositionsAll.size(); i++)
    {
        lZ += lPositionsAll[i].z;
    }
    
    lZ = lZ / (float)lPositionsAll.size();
//    console() << lZ << std::endl;
    
    // draw anchors
    float gap = lPositionsAll.size() / Shared::sAnchors.size();
    int lIndex;
    for(int i = 0; (i < lPositionsAll.size()) && (i < Shared::sAnchors.size()); i++)
    {
        lIndex = floor((float)i * gap);
        Shared::sAnchors[i]->SetPosition(lPositionsAll[lIndex]);
        Shared::sAnchors[i]->mColor = lColorsAll[lIndex];
    }
}

void KinectManager::Calibrate()
{
    mCalibCount = 24;
    console() << "calibrate kinect" << endl;
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
        if(mCalibCount <= 0)
        {
            console() << "calibration done" << endl;
        }
        float lDistanceCalib, lDistanceCurrent;
        
        // we'll need to iterate the inputSurface as well as the output surface
        Surface::Iter inputIter( mDepthSurface.getIter() );
        Surface::Iter calibIter( mPixelsCalib.getIter(  ) );
        
        while( inputIter.line() && calibIter.line() ) {
            while( inputIter.pixel() && calibIter.pixel() ) {
                lDistanceCalib = calibIter.r();
                lDistanceCurrent = inputIter.r();
                if(lDistanceCurrent >= 254)
                {
                    calibIter.r() = 0;
                }
                else
                {
                    calibIter.r() = max(lDistanceCalib, lDistanceCurrent);
                }
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
    if(!isCalibrated())
    {
        gl::drawString("CALIBRATING", Vec2f(Tools::getWindowWidth(),Tools::getWindowHeight()) * 0.5f);
    }
    drawDebug(0,0,Tools::getWindowWidth(),Tools::getWindowHeight());
    
    Set::draw();
}

void KinectManager::drawDebug(float x, float y, float w, float h){

}
