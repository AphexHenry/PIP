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
#include "SoundPlayer.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImagePlayer : public VideoPlayer {
public:
    
    ImagePlayer(fs::path img, fs::path imgFront);
    ImagePlayer(fs::path img, fs::path imgFront, std::string aSoundFile);
    
    void SetupImages(fs::path img, fs::path imgFront);
    
	void update(float aTimeInterval);
    
    void play();
    void stop();
    bool isDone();
    
    float GetTimeLeft();
    float GetTimeCurrent();
    
    void SetVolume(float aVolume);
    
    SoundPlayer * mSoundPlayer;
};

#endif /* defined(__StereoscopicRendering__ImagePlayer__) */
