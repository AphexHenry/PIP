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

VideoPlayer::VideoPlayer(fs::path movie, fs::path img)
{
    fs::path moviePath = App::getResourcePath( movie ) ;
    loadMovieFile( moviePath );
    mImageFrontReflection = gl::Texture(loadImage( App::getResourcePath(img)));
    
    mTimeToPlay = 25.f;
}

VideoPlayer::VideoPlayer(fs::path movie)
{
    fs::path moviePath = App::getResourcePath( movie ) ;
    loadMovieFile( moviePath );
    
    mTimeToPlay = 25.f;
}

void VideoPlayer::setFront(fs::path aImgFront)
{
    mImageFront = gl::Texture(loadImage( App::getResourcePath(aImgFront)));
}

void VideoPlayer::setup()
{
    //	fs::path moviePath = getOpenFilePath();
    //	if( ! moviePath.empty() )
    //		loadMovieFile( moviePath );
//    fs::path moviePath = ci::getResourcePath( FILM_1 ) ;
//    loadMovieFile( moviePath );
}

void VideoPlayer::setImage( gl::Texture &aTexture )
{
    mImageFrontReflection = aTexture;
}

void VideoPlayer::loadMovieFile( const fs::path &moviePath )
{
	try {
		// load up the movie, set it to loop, and begin playing
//		mMovie = qtime::MovieSurface( moviePath );
        mMovie = qtime::MovieGl::create( moviePath );
//        mMovie->setLoop();
	}
	catch( ... ) {
		console() << "Unable to load the movie." << std::endl;
		mMovie->reset();
	}
    
	mFrameTexture.reset();
}

void VideoPlayer::update(float aTimeInterval)
{
	if( mMovie )
    {
        mTimeToPlay -= aTimeInterval;
        
        if(mMovie->checkNewFrame())
        {
            mFrameTexture = mMovie->getTexture();
        }
//        mSurface = copyWindowSurface();//TextureToSurface(mFrameTexture).clone();
//        mFrameTexture = gl::Texture(mSurface);
//        }
//        else
//        {
//            mFrameTexture.update(mSurface);
//        }
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
    mMovie->stop();
    mMovie->seekToStart();
    mMovie->play();
    
    mTimeToPlay = 45.f;
}

void VideoPlayer::stop()
{
    mMovie->stop();
    mFrameTexture = gl::Texture();
}

bool VideoPlayer::isDone()
{
//#ifdef MY_APP
    return mTimeToPlay < 0.f || mMovie->isDone();
//#else
//    return ;
//#endif
}

void VideoPlayer::SetVolume(float aVolume)
{
    mMovie->setVolume(aVolume);
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