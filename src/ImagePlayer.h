//
//  VideoPlayer.h
//  StereoscopicRendering
//
//  Created by Baptiste Bohelay on 2014-01-14.
//
//

#ifndef __StereoscopicRendering__ImagePlayer__
#define __StereoscopicRendering__ImagePlayer__

#include "VideoPlayer.h"
#include "cinder/audio/audio.h"
#include "cinder/audio/Io.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImagePlayer : public VideoPlayer {
public:
    
    ImagePlayer(fs::path img, fs::path imgFront, float aDuration);
    ImagePlayer(fs::path img, fs::path imgFront, std::string aSoundFile, float aDuration);
    
	void update(float aTimeInterval);
    
    void play();
    void stop();
    bool isDone();
    
    void SetVolume(float aVolume);
    
    float mTimeToPlay;
    float mDuration;
    
    audio::GainNodeRef				mGain;
    audio::BufferPlayerNodeRef		mBufferPlayerNode;
//    ci::audio::TrackRef mTrack;
};

#endif /* defined(__StereoscopicRendering__ImagePlayer__) */
