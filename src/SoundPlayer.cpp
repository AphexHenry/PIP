//
//  SoundPlayer.cpp
//  StereoscopicRendering
//
//  Created by Baptiste Bohelay on 2015-02-03.
//
//

#include "SoundPlayer.h"
#include "cinder/app/AppBasic.h"
#include "Tools.h"
#include "Resources.h"

using namespace ci;
using namespace std;

SoundPlayer::SoundPlayer(std::string aFilePath)
{
    auto ctx = audio::Context::master();
    
    // create a SourceFile and set its output samplerate to match the Context.
    app::addAssetDirectory	(	app::getAppPath().string() + "/../Assets/"	 );
//    std::string lResource = app::getAppPath().string() + "/../Assets/" + aFilePath;
    DataSourceRef lRef = app::loadAsset(  aFilePath);
    app::console() << lRef->getFilePath().string() << endl;
    audio::SourceFileRef sourceFile = audio::load(lRef , ctx->getSampleRate() );
    
    // load the entire sound file into a BufferRef, and construct a BufferPlayerNode with this.
    audio::BufferRef buffer = sourceFile->loadBuffer();
    mBufferPlayerNode = ctx->makeNode( new audio::BufferPlayerNode( buffer ) );
    mBufferPlayerNode->setLoopEnabled(true);
    
    mGain = ctx->makeNode( new audio::GainNode( 0.5f ) );
    
    // connect and enable the Context
    mBufferPlayerNode >> mGain >> ctx->getOutput();
    ctx->enable();

}

void SoundPlayer::setVolume(float aVolume)
{
    mGain->setValue( aVolume );
}

void SoundPlayer::play()
{
    if(mBufferPlayerNode)
    {
        mBufferPlayerNode->start();
    }
}

void SoundPlayer::stop()
{
    if(mBufferPlayerNode)
    {
        mBufferPlayerNode->stop();
    }
}