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
    
    /*
     *  Constructor including reflection mask.
     */
    VideoPlayer(fs::path movie, fs::path img);
    
    /*
     *  Constructor, will only load the movie file.
     */
    VideoPlayer(fs::path movie);
    
    /*
     *  Constructor, will load nothing.
     */
    VideoPlayer(){};
    
	virtual void update(float aTimeInterval);
	virtual void draw(Vec2i aSize);
    
    /*
     *  set the front mask image.
     */
    void setFront(fs::path imgFront);
    void drawFront(Vec2i aSize);
    void drawFrontReflection(Vec2i aSize);
    
    virtual void play();
    virtual void stop();
    virtual bool isDone();
    virtual void SetVolume(float aVolume);
    virtual void SetDuration(float aDurationS){mDuration = aDurationS;};
    
    static bool IsMovie(std::string aPath);
    
    void setImage( gl::Texture &aTexture );
	void loadMovieFile( const fs::path &path );
    
    Surface                 mSurface;
	gl::Texture             mFrameTexture;
	qtime::MovieGlRef       mMovie;
    gl::Texture             mImageFrontReflection;
    gl::Texture             mImageFront;
    
    float mTimeToPlay;
    float mDuration;
    
    float mVolume;
    
    fs::path mMoviePath;
    
private:
    void setup(fs::path aPath);
    Surface8u TextureToSurface(gl::Texture aTexture);
};

#endif /* defined(__StereoscopicRendering__VideoPlayer__) */
