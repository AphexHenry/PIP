//
//  VideoPlayer.cpp
//  StereoscopicRendering
//
//  Created by Baptiste Bohelay on 2014-01-14.
//
//

#include "cinder/app/App.h"
#include "ImagePlayer.h"
#include "Resources.h"
#include "cinder/Utilities.h"

using namespace ci;
using namespace ci::app;

ImagePlayer::ImagePlayer(fs::path img, fs::path imgFront, float aDuration)
{

    auto ctx = audio::Context::master();
    
    // create a SourceFile and set its output samplerate to match the Context.
    audio::SourceFileRef sourceFile = audio::load( loadResource( RES_DRAIN_OGG ), ctx->getSampleRate() );
    
    // load the entire sound file into a BufferRef, and construct a BufferPlayerNode with this.
    audio::BufferRef buffer = sourceFile->loadBuffer();
    mBufferPlayerNode = ctx->makeNode( new audio::BufferPlayerNode( buffer ) );
    
    // add a Gain to reduce the volume
    mGain = ctx->makeNode( new audio::GainNode( 0.5f ) );
    
    // connect and enable the Context
    mBufferPlayerNode >> mGain >> ctx->getOutput();
    ctx->enable();
    
    
    mFrameTexture = gl::Texture(loadImage( App::getResourcePath(img)));
    if(!imgFront.empty())
    {
        mImageFront = gl::Texture(loadImage( App::getResourcePath(imgFront)));
    }
    mSurface = Surface(mFrameTexture);
    mTimeToPlay = aDuration;
    mDuration = aDuration;

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
    ivec2 lpos = ivec2(Rand::randInt(mSurface.getWidth()), Rand::randInt(mSurface.getHeight()));
    Shared::sColorCapture[0] = mSurface.getPixel(lpos);
    Shared::sColorCapture[1] = mSurface.getPixel(ivec2(Rand::randInt(mSurface.getWidth()), Rand::randInt(mSurface.getHeight())));
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
#ifdef MY_APP
   return mTimeToPlay < 0.f;
#else
    if(mTrack)
    {
        return !mTrack->isPlaying();
    }
    else
    {
        return mTimeToPlay < 0.f;
    }
#endif
}
