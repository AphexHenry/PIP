
#include "cinder/Rand.h"
#include "CaptureManager.h"

using namespace ci;
using namespace ci::app;

/*
 *  return true if camera is available, false otherwise.
 */
bool CaptureManager::IsAvailable()
{
    // return false if there is 0 capture device available.
    return Capture::getDevices().size() > 0;
}

/*
 *  setup the computer camera.
 *  return false if none can be found.
 */
bool CaptureManager::setup()
{
	try {
		mCapture = Capture::create( 640, 480 );
		mCapture->start();
	}
	catch( ... ) {
		console() << "Failed to initialize capture" << std::endl;
        return false;
	}
    
    return true;
}

/*
 *  update the capture image.
 */
void CaptureManager::update(float aTimeInterval)
{
	if( mCapture && mCapture->checkNewFrame() ) {
		mSurface =  mCapture->getSurface();
	}
}

/*
 *  return true when the capture can start.
 */
bool CaptureManager::isReady()
{
    // if there is a new frame, the capture manager is ready.
    return mCapture->checkNewFrame() && mCapture->isCapturing();
}

/*
 *  draw the camera image.
 *  debug purpose.
 */
void CaptureManager::draw()
{
    if(!mSurface)
        return;
	gl::clear( Color( 0.0f, 0.0f, 0.0f ) );
	gl::setMatricesWindow( getWindowWidth(), getWindowHeight() );
	
    gl::TextureRef lTexture = gl::Texture::create(mSurface);
    
	if( lTexture ) {
		glPushMatrix();
		gl::draw( lTexture );
		glPopMatrix();
	}
}

/*
 *  start calibration.
 *  in case of this camera, the calibration is done every frame.
 */
void CaptureManager::Calibrate()
{
    // reset calib surface.
    if(mSurface)
    {
        mPixelsCalib = mSurface;
    }
}

/*
 *  update the anchors positions.
 */
void CaptureManager::UpdateAnchors()
{
    // if no calibration surface, start calibration.
    if(!mSurface || !mPixelsCalib)
    {
        Calibrate();
        return;
    }
    vector<Vec3f> lPositionsAll;
    float lDistanceCalib;
    
    // we'll need to iterate the inputSurface as well as the output surface
    Surface::Iter inputIter( mSurface.getIter() );
    Surface::Iter calibIter( mPixelsCalib.getIter(  ) );
    
    int numIter = 0;
    float luminositySum = 0.f;
    
    while( inputIter.line() && calibIter.line() ) {
        if(!inputIter.line() || !calibIter.line())
        {
            break;
        }
        while( inputIter.pixel() && calibIter.pixel() ) {
            if(!inputIter.pixel() || !calibIter.pixel())
            {
                break;
            }
            // position of current pixel.
            Vec2f current = inputIter.getPos();
            
            // get rgb distance of current capture surface from calibration surface.
            lDistanceCalib = fabs((calibIter.r() - inputIter.r())) + fabs((calibIter.g() - inputIter.g())) + fabs((calibIter.b() - inputIter.b()));
            
            // check if this distance is higher than the threshold, defined by the ambient luminosity.
            if(lDistanceCalib > 1.3f * mAmbientLuminosity)
            {
                // if distance is high, it means this pixel has changed, then we can add it to the vector containing all moving pixels.
                lPositionsAll.push_back(Vec3f(current.x / (float)mSurface.getWidth(),(float)current.y / (float)mSurface.getWidth(),Rand::randFloat(-0.5f, 0.5f)));
            }
            
            // slowly make calibration surface match current surface.
            calibIter.r() += (inputIter.r() - calibIter.r()) * 0.02f;
            calibIter.g() += (inputIter.g() - calibIter.g()) * 0.02f;
            calibIter.b() += (inputIter.b() - calibIter.b()) * 0.02f;
            
            // add this pixel to thr current luminosity.
            luminositySum += inputIter.r() + inputIter.g() + inputIter.b();
            numIter++;
        }
    }
    
    // do the average of all the pixels luminosity.
    mAmbientLuminosity = luminositySum / ((float)numIter);
    
    // only make anchors valid if movement is big enough. An almost still capture has to only show the media.
    int lChangeMin = floor((float)mSurface.getWidth() * (float)mSurface.getHeight() * 0.0005f);
    if(lPositionsAll.size() > lChangeMin)
    {
        // draw anchors
        float gap = lPositionsAll.size() / Shared::sAnchors.size();
        for(int i = 0; (i < lPositionsAll.size()) && (i < Shared::sAnchors.size()); i++)
        {
            Shared::sAnchors[i]->SetPosition(lPositionsAll[floor((float)i * gap)]);
        }
    }
    else
    {
        for(int i = 0; (i < Shared::sAnchors.size()); i++)
        {
            Shared::sAnchors[i]->SetPosition(Vec3f(100.5f + Rand::randFloat(1.f), 10.f, 0.f));
        }
    }
}
