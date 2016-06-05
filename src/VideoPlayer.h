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

#include "cinder/app/App.h"
#include "cinder/Surface.h"
#include "cinder/gl/Texture.h"
#include "cinder/qtime/QuickTimeGl.h"
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
	virtual void draw(ivec2 aSize);
    void setFront(fs::path imgFront);
    void drawFront(ivec2 aSize);
    void drawFrontReflection(ivec2 aSize);
    
    virtual void play();
    virtual void stop();
    virtual bool isDone();
    virtual void SetVolume(float aVolume);
    
    void setImage( gl::Texture &aTexture );
	void loadMovieFile( const fs::path &path );
    
    Surface                 mSurface;
	gl::TextureRef             mFrameTexture;
	qtime::MovieGlRef           mMovie;
    gl::TextureRef             mImageFrontReflection;
    gl::TextureRef             mImageFront;
    
    float mTimeToPlay;
    
private:
    Surface8u TextureToSurface(gl::Texture aTexture);
};

#endif /* defined(__StereoscopicRendering__VideoPlayer__) */
