//
//  VideoPlayer.h
//  StereoscopicRendering
//
//  Created by Baptiste Bohelay on 2014-01-14.
//
//

#ifndef __StereoscopicRendering__VideoPlayer__
#define __StereoscopicRendering__VideoPlayer__

#include <iostream>

#include "cinder/app/AppBasic.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTime.h"
#include "cinder/Text.h"
#include "cinder/Utilities.h"
#include "cinder/ImageIo.h"
#include "cinder/Rand.h"
#include "shared.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class VideoPlayer {
public:
    
    VideoPlayer(fs::path movie, fs::path img);
    VideoPlayer(fs::path movie);
    VideoPlayer(){};
    
	void setup();
    
	virtual void update(float aTimeInterval);
	virtual void draw(Vec2i aSize);
    void setFront(fs::path imgFront);
    void drawFront(Vec2i aSize);
    void drawFrontReflection(Vec2i aSize);
    
    virtual void play();
    virtual void stop();
    virtual bool isDone();
    virtual void SetVolume(float aVolume);
    
    void setImage( gl::Texture &aTexture );
	void loadMovieFile( const fs::path &path );
    
    Surface                 mSurface;
	gl::Texture             mFrameTexture;
	qtime::MovieGlRef       mMovie;
    gl::Texture             mImageFrontReflection;
    gl::Texture             mImageFront;
    
    float mTimeToPlay;
    
private:
    Surface8u TextureToSurface(gl::Texture aTexture);
};

#endif /* defined(__StereoscopicRendering__VideoPlayer__) */
