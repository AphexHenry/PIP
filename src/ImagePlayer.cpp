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

#include "cinder/audio/Context.h"
#include "cinder/audio/NodeEffects.h"
#include "cinder/audio/SamplePlayerNode.h"

using namespace ci;
using namespace ci::app;

ImagePlayer::ImagePlayer(fs::path img, fs::path imgFront)
{
    SetupImages(img, imgFront );

    mSoundPlayer = NULL;
    
}

ImagePlayer::ImagePlayer(fs::path img, fs::path imgFront, std::string aSoundFile)
{
    SetupImages(img, imgFront );
    
    if(aSoundFile.length() > 0)
    {
        mSoundPlayer = new SoundPlayer(aSoundFile);
    }
}

void ImagePlayer::SetupImages(fs::path img, fs::path imgFront)
{
    console() << "img " << img << endl;
    console() << imgFront << endl;
    //add the audio track the default audio output
    
    mFrameTexture = gl::Texture(loadImage( Tools::GetOutResourcePath(img)));
    if(!imgFront.empty())
    {
        mImageFront = gl::Texture(loadImage( Tools::GetOutResourcePath(imgFront)));
    }
    mSurface = Surface(mFrameTexture);
    mTimeToPlay = -1.f;
    mDuration = 10.f;
}

void ImagePlayer::SetVolume(float aVolume)
{
    if(mSoundPlayer)
    {
        mSoundPlayer->setVolume( aVolume );
    }
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
    
    if(mSoundPlayer)
    {
        mSoundPlayer->play( );
    }
}

void ImagePlayer::stop()
{
    if(mSoundPlayer)
    {
        mSoundPlayer->stop( );
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
