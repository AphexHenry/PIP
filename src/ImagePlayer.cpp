//
//  VideoPlayer.cpp
//  StereoscopicRendering
//
//  Created by Baptiste Bohelay on 2014-01-14.
//
//

#include "cinder/app/AppBasic.h"
#include "ImagePlayer.h"
#include "Resources.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;

ImagePlayer::ImagePlayer(fs::path img, fs::path imgFront, float aDuration)
{
    console() << "imgFront " << img << endl;
    console() << imgFront << endl;
    mFrameTexture = gl::Texture(loadImage( App::getResourcePath(img)));
    if(!imgFront.empty())
    {
        mImageFront = gl::Texture(loadImage( App::getResourcePath(imgFront)));
    }
    mSurface = Surface(mFrameTexture);
    mTimeToPlay = aDuration;
    mDuration = aDuration;
#ifdef MY_APP
    mDuration *= 0.75f;
#endif

}

ImagePlayer::ImagePlayer(fs::path img, fs::path imgFront, std::string aSoundFile, float aDuration)
{
    console() << "img " << img << endl;
    console() << imgFront << endl;
    //add the audio track the default audio output
	mTrack = audio::Output::addTrack( audio::load( loadResource( aSoundFile ) ), false );
    mTrack->setLooping(true);
    
    mFrameTexture = gl::Texture(loadImage( App::getResourcePath(img)));
    if(!imgFront.empty())
    {
        mImageFront = gl::Texture(loadImage( App::getResourcePath(imgFront)));
    }
    mSurface = Surface(mFrameTexture);
    mTimeToPlay = aDuration;
    mDuration = aDuration;
#ifdef MY_APP
    mDuration *= 0.75f;
#endif
}

void ImagePlayer::SetVolume(float aVolume)
{
    mTrack->setVolume(aVolume);
}

void ImagePlayer::update(float aTimeInterval)
{
    Vec2i lpos = Vec2i(Rand::randInt(mSurface.getWidth()), Rand::randInt(mSurface.getHeight()));
    Shared::sColorCapture[0] = mSurface.getPixel(lpos);
    Shared::sColorCapture[1] = mSurface.getPixel(Vec2i(Rand::randInt(mSurface.getWidth()), Rand::randInt(mSurface.getHeight())));
    mTimeToPlay -= aTimeInterval;
}

void ImagePlayer::play()
{
    mTimeToPlay = mDuration;
    if(mTrack)
    {
        mTrack->play();
    }
}

void ImagePlayer::stop()
{
    if(mTrack)
    {
        mTrack->stop();
    }
}

bool ImagePlayer::isDone()
{
//#ifdef MY_APP
   return mTimeToPlay < 0.f;
//#else
//    if(mTrack)
//    {
//        return !mTrack->isPlaying();
//    }
//    else
//    {
//        return mTimeToPlay < 0.f;
//    }
//#endif
}
