//
//  SoundPlayer.h
//  StereoscopicRendering
//
//  Created by Baptiste Bohelay on 2015-02-03.
//
//

#ifndef __StereoscopicRendering__SoundPlayer__
#define __StereoscopicRendering__SoundPlayer__

#include <stdio.h>

#include "cinder/audio/Context.h"
#include "cinder/audio/NodeEffects.h"
#include "cinder/audio/SamplePlayerNode.h"

class SoundPlayer
{
public:
    SoundPlayer(std::string aSoundFile);
    void setVolume(float aVolume);
    void play();
    void stop();
    
private:
    ci::audio::GainNodeRef				mGain;
    ci::audio::BufferPlayerNodeRef		mBufferPlayerNode;
};

#endif /* defined(__StereoscopicRendering__SoundPlayer__) */
