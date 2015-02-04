
#include "cinder/Rand.h"
#include "CaptureManager.h"

using namespace ci;
using namespace ci::app;

bool CaptureManager::IsAvailable()
{
    return Capture::getDevices().size() > 0;
}

bool CaptureManager::setup()
{
	// print the devices
	for( auto device = Capture::getDevices().begin(); device != Capture::getDevices().end(); ++device ) {
		console() << "Device: " << (*device)->getName() << " "
#if defined( CINDER_COCOA_TOUCH )
					<< ( (*device)->isFrontFacing() ? "Front" : "Rear" ) << "-facing"
#endif
					<< std::endl;
	}

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

void CaptureManager::update(float aTimeInterval)
{
	if( mCapture && mCapture->checkNewFrame() ) {
		mSurface =  mCapture->getSurface();
	}
}

bool CaptureManager::isReady()
{
    return mCapture->checkNewFrame() && mCapture->isCapturing();
}

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

void CaptureManager::Calibrate()
{
    if(mSurface)
    {
        mPixelsCalib = mSurface;
    }
}

void CaptureManager::UpdateAnchors()
{
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
                
            Vec2f current = inputIter.getPos();
            
            lDistanceCalib = fabs((calibIter.r() - inputIter.r())) + fabs((calibIter.g() - inputIter.g())) + fabs((calibIter.b() - inputIter.b()));
            
//            lDistanceCalib = (float)(calibIter.r() + calibIter.g() + calibIter.b()) / (float)(inputIter.r() + inputIter.g() + inputIter.b());
            
            if(lDistanceCalib > 1.3f * mAmbientLuminosity)
            {
                lPositionsAll.push_back(Vec3f(current.x / (float)mSurface.getWidth(),(float)current.y / (float)mSurface.getWidth(),Rand::randFloat(-0.5f, 0.5f)));
            }
            
            calibIter.r() += (inputIter.r() - calibIter.r()) * 0.02f;
            calibIter.g() += (inputIter.g() - calibIter.g()) * 0.02f;
            calibIter.b() += (inputIter.b() - calibIter.b()) * 0.02f;
            
            luminositySum += inputIter.r() + inputIter.g() + inputIter.b();
            numIter++;
        }
    }
    
    mAmbientLuminosity = luminositySum / ((float)numIter);
    
    // draw anchors
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
