//
//  VideoPlayer.cpp
//  StereoscopicRendering
//
//  Created by Baptiste Bohelay on 2014-01-14.
//
//

#include "VideoPlayer.h"
#include "cinder/Utilities.h"
#include "cinder/gl/Fbo.h"

using namespace ci;
using namespace ci::app;


ThreadLoader VideoPlayer::mLoader;

/*
 *  Constructor including reflection mask.
 */
VideoPlayer::VideoPlayer(fs::path movie, fs::path img)
{
    mImageFrontReflection = gl::Texture(loadImage( Tools::GetOutResourcePath(img)));
    setup(movie);
}

VideoPlayer::VideoPlayer(fs::path movie)
{
    setup(movie);
}

/*
 *  set the front mask image.
 */
void VideoPlayer::setFront(fs::path aImgFront)
{
    mImageFront = gl::Texture(loadImage( Tools::GetOutResourcePath(aImgFront)));
}

/*
 *  common setup function for both constructors.
 */
void VideoPlayer::setup(fs::path movie)
{
    fs::path moviePath = Tools::GetOutResourcePath( movie ) ;
    mMoviePath = moviePath;
    
    mTimeToPlay = -1.f;
    mDuration = -1.f;
    mVolume = 1.f;
}

/*
 *  set the reflection mask image.
 */
void VideoPlayer::setImage( gl::Texture &aTexture )
{
    mImageFrontReflection = aTexture;
}

/*
 *  load movie into memory.
 */
void VideoPlayer::loadMovieFile( const fs::path &moviePath )
{
//	try {
//		// load up the movie, set it to loop, and begin playing
////		mMovie = qtime::MovieSurface( moviePath );
//        mMovie = qtime::MovieGl::create( moviePath );
////        mMovie->setActiveSegment( 0.f, 1.f );
////        mMovie->setLoop();
//	}
//	catch( ... ) {
//		console() << "Unable to load the movie." << std::endl;
//		mMovie->reset();
//	}

    mLoader.load(moviePath);
    
	mFrameTexture.reset();
}

void VideoPlayer::update(float aTimeInterval)
{
    std::list< ci::qtime::MovieGlRef > lLoadedList = mLoader.getLoaded();
    if(lLoadedList.size() > 0)
    {
        mMovie = *(lLoadedList.begin());
        
        if(mMovie->checkPlayable())
        {
            //        mMovie->stop();
            mMovie->seekToStart();
            mMovie->play();
            mMovie->setVolume(mVolume);
        }
        else
        {
            console() << "movie not playable" << endl;
        }
        
        mTimeToPlay = mDuration;
    }
    
	if( mMovie )
    {
        mTimeToPlay -= aTimeInterval;
        
        if(mMovie->checkNewFrame())
        {
            mFrameTexture = mMovie->getTexture();
        }
#ifndef MY_APP
        if(!mSurface)
        {
            draw(AppBasic::get()->getWindowSize());
            mSurface = copyWindowSurface();
        }
        else
        {
            Vec2i lpos = Vec2i(Rand::randInt(mSurface.getWidth()), Rand::randInt(mSurface.getHeight()));
            Shared::sColorCapture[0] = mSurface.getPixel(lpos);
            Shared::sColorCapture[1] = mSurface.getPixel(Vec2i(Rand::randInt(mSurface.getWidth()), Rand::randInt(mSurface.getHeight())));
        }
#endif
    }
}

void VideoPlayer::draw(Vec2i aSize)
{
    gl::enableAlphaBlending();
    
	if( mFrameTexture ) {
        float lratioImg = mFrameTexture.getWidth() / mFrameTexture.getHeight();//Rectf( mFrameTexture.getBounds() );
        float lheight = ((aSize.x / lratioImg) - aSize.y) * 0.5f;
        Rectf centeredRect = Rectf(0, -lheight, aSize.x, aSize.y + lheight);
//		Rectf centeredRect = Rectf( mFrameTexture.getBounds() ).getCenteredFit( Area(0, 0, aSize.x, aSize.y), true );
		gl::draw( mFrameTexture, centeredRect );
	}
}

void VideoPlayer::play()
{
    loadMovieFile(mMoviePath);
}

void VideoPlayer::stop()
{
//    mMovie->stop();
    mMovie->reset();
    mMovie = nullptr;
    
    mFrameTexture = gl::Texture();
}

bool VideoPlayer::isDone()
{
    if(!mMovie)
    {
        return false;
    }
    return ((mDuration > 0.f) && (mTimeToPlay < 0.f)) || mMovie->isDone();
}

void VideoPlayer::SetVolume(float aVolume)
{
    mVolume = aVolume;
}

float VideoPlayer::GetTimeLeft()
{
    if(!mMovie)
    {
        return 0.f;
    }
    
    if(mDuration > 0.f)
    {
        return mTimeToPlay;
    }
    else
    {
        return mMovie->getDuration() - mMovie->getCurrentTime();
    }
}

float VideoPlayer::GetTimeCurrent()
{
    if(!mMovie)
    {
        return 0.f;
    }
    
    return mMovie->getCurrentTime();
}

void VideoPlayer::drawFrontReflection(Vec2i aSize)
{
    if( mFrameTexture && mImageFrontReflection)
    {
        float lratioImg = mFrameTexture.getWidth() / mFrameTexture.getHeight();//Rectf( mFrameTexture.getBounds() );
        float lheight = ((aSize.x / lratioImg) - aSize.y) * 0.5f;
        Rectf centeredRect = Rectf(0, -lheight, aSize.x, aSize.y + lheight);//Rectf( mFrameTexture.getBounds() ).getCenteredFit( Area(0, 0, aSize.x, aSize.y), true );
        gl::draw( mImageFrontReflection, centeredRect  );
    }
}

void VideoPlayer::drawFront(Vec2i aSize)
{
    if( mFrameTexture && mImageFront)
    {
        float lratioImg = mFrameTexture.getWidth() / mFrameTexture.getHeight();//Rectf( mFrameTexture.getBounds() );
        float lheight = ((aSize.x / lratioImg) - aSize.y) * 0.5f;
        Rectf centeredRect = Rectf(0, -lheight, aSize.x, aSize.y + lheight);
//        Rectf centeredRect = Rectf( mFrameTexture.getBounds() ).getCenteredFit( Area(0, 0, aSize.x, aSize.y), true );//getWindowBounds()
        gl::draw( mImageFront, centeredRect  );
    }
}

Surface8u VideoPlayer::TextureToSurface(gl::Texture aTexture)
{
    glEnable(GL_TEXTURE_2D);
    gl::Fbo lFbo = gl::Fbo(512, 512, gl::Fbo::Format());
    
    // Set up to copy
    //gl::clear(Color(0.0f, 0.0f, 0.0f));
    gl::setMatricesWindow(lFbo.getSize(), false);
    gl::setViewport(lFbo.getBounds());
    lFbo.bindFramebuffer();
    // Draw the camera input texture to the FBO, scaling it to fit
    gl::draw(aTexture, lFbo.getBounds());
    Surface fboSurf = copyWindowSurface();
    lFbo.unbindFramebuffer();
    
    return fboSurf;
}

bool VideoPlayer::IsMovie(string aPath)
{
    vector<string> lListExtensionMovie;
    lListExtensionMovie.push_back("avi");
    lListExtensionMovie.push_back("AVI");
    lListExtensionMovie.push_back("mov");
    lListExtensionMovie.push_back("MOV");
    lListExtensionMovie.push_back("mp4");
    lListExtensionMovie.push_back("MP4");
    lListExtensionMovie.push_back("m4v");
    lListExtensionMovie.push_back("M4V");
    string lMediaExtension = getPathExtension(aPath);
    for(int i = 0; i < lListExtensionMovie.size(); i++)
    {
        if(lMediaExtension == lListExtensionMovie[i])
        {
            return true;
        }
    }
    return false;
}