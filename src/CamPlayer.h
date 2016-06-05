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
#include "cinder/audio/Output.h"
#include "cinder/audio/Io.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class CamPlayer : public VideoPlayer {
public:
    
    CamPlayer(fs::path imgFront, float aDuration);
    
	void update(float aTimeInterval);
    
    void play();
    void stop();
    bool isDone();
    
    void SetVolume(float aVolume){};
    
    float mTimeToPlay;
    float mDuration;
};

#endif /* defined(__StereoscopicRendering__ImagePlayer__) */
