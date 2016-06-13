/*
 *  Tools.cpp
 *  SurfaceBasic
 *
 *  Created by TI mini three on 12-03-19.
 *  Copyright 2012 Tangible Interaction Design Inc. All rights reserved.
 *
 */

#include "Set.h"
#include "Resources.h"
#include "ParticleBillBoardTree.h"
#include "ImagePlayer.h"
#include "TransitionManager.h"

#include "cinder/Xml.h"

using namespace std;
using namespace ci;

float Set::frictionStr;
float Set::gravity;
float Set::strengthMove;
float Set::frameRate;
float Set::heratic;
float Set::heratic2;
float Set::lineVisibility;
float Set::particleSize = 0.007f;
float Set::particleStrCoeff;
float Set::coeffZKinect = -1.f / 25.f;
bool  Set::mirror;
float  Set::compression = 1.f;
map<string, ParticleType> Set::particleMapper;

SoundPlayer * Set::sSoundPlayer = NULL;

int     Set::currentScene = 0;
vector<SceneData> Set::scenes;

bool  Set::hide = true;

void Set::ToggleHide()
{
    hide = !hide;
}

void Set::setup(SensorType aSensorType)
{
    gl::Texture::Format format;
	format.setMinFilter( GL_NEAREST );
	format.setMagFilter( GL_NEAREST );
	format.setInternalFormat( GL_RGBA );
    ParticleBillboardTree::AddTexture(gl::Texture(loadImage( loadResource(PARTICLE_TREE_1))));
    ParticleBillboardTree::AddTexture(gl::Texture(loadImage( loadResource(PARTICLE_TREE_2))));
    ParticleBillboardTree::AddTexture(gl::Texture(loadImage( loadResource(PARTICLE_TREE_3))));
    ParticleBillboardTree::AddTexture(gl::Texture(loadImage( loadResource(PARTICLE_TREE_4))));
    ParticleBillboardTree::AddTexture(gl::Texture(loadImage( loadResource(PARTICLE_TREE_5))));
    
    particleMapper["circle"] = PARTICLE_TYPE_SPHERE;
    particleMapper["line"] = PARTICLE_TYPE_LINE;
    particleMapper["tree"] = PARTICLE_TYPE_IMG;
    
    SceneData lScene;
    
    XmlTree lTree(loadAsset("_Setup.xml"));
    
    if(lTree.hasChild("Ambient"))
    {
        XmlTree globalList = lTree.getChild( "Ambient" );
        
        for( XmlTree::Iter child = globalList.begin(); child != globalList.end(); ++child )
        {
            if(child->getTag() == "audio")
            {
                string lFile = child->getAttributeValue<string>( "file", "" );
                float lVolume = child->getAttributeValue<float>( "volume", 1.f );
                if(lFile.length() > 0)
                {
                    sSoundPlayer = new SoundPlayer(lFile);
                    sSoundPlayer->play();
                    sSoundPlayer->setVolume(lVolume);
                }
            }
            else if(child->getTag() == "transition")
            {
                string lTransitionType = child->getAttributeValue<string>( "type", "none" );
                float lDuration = child->getAttributeValue<float>("duration", 0.7f);
                if(lTransitionType == "FadeToBlack")
                {
                    TransitionManager::SetTransition(TRANSITION_TYPE_FADE_TO_BLACK, lDuration);
                }
            }
        }
    }
    
    XmlTree sceneList = lTree.getChild( "Composition" );
    
    for( XmlTree::Iter child = sceneList.begin(); child != sceneList.end(); ++child )
    {
        lScene.particles = NULL;
        for(XmlTree::Iter lSceneChild = child->begin(); lSceneChild != child->end(); ++lSceneChild )
        {
            if(lSceneChild->getTag() == "particles")
            {
                int count = lSceneChild->getAttributeValue<int>( "count", 400 );
                lScene.particleType = particleMapper[lSceneChild->getAttributeValue<string>( "type", "circle" )];
                lScene.position.x = lSceneChild->getAttributeValue<float>( "x", 0 );
                lScene.position.y = lSceneChild->getAttributeValue<float>( "y", 0 );
                lScene.position.z = lSceneChild->getAttributeValue<float>( "z", 0 );
//                lScene.eyesSeparation = lSceneChild->getAttributeValue<float>( "z", 0 ) / 1000.f;
                lScene.scale = lSceneChild->getAttributeValue<float>( "size", 0 );
                lScene.opacity = lSceneChild->getAttributeValue<float>( "opacity", 1.f );
                lScene.movement = Vec3f(1.f, 1.f, 1.f) * lSceneChild->getAttributeValue<float>( "scale", 1.f );
                lScene.reflection = lSceneChild->getAttributeValue<bool>( "reflection", false );
                lScene.particleDuration = lSceneChild->getAttributeValue<float>("lifeTime", 0.5f);
                lScene.intensityMin = lSceneChild->getAttributeValue<float>( "brightnessMin", 0.f );
                lScene.intensityMax = lSceneChild->getAttributeValue<float>( "brightnessMax", 1.f );
                lScene.colorMin = Tools::stringToColor(lSceneChild->getAttributeValue<string>( "colorMin", "0,0,0" ));
                lScene.colorMax = Tools::stringToColor(lSceneChild->getAttributeValue<string>( "colorMax", "1,1,1" ));
                lScene.useKinectColor = lSceneChild->getAttributeValue<bool>( "useKinectColor", false );
                lScene.useVideoColor = lSceneChild->getAttributeValue<bool>( "useVideoColor", false );
                lScene.distanceSensitivity = lSceneChild->getAttributeValue<float>( "distanceSensitivity", 0.25f );
                
                lScene.particles = new vector<Particle *>();
                for(int i = 0; i < count; i++)
                {
                    if(lScene.particleType == PARTICLE_TYPE_IMG)
                    {
                        lScene.particles->push_back(new ParticleBillboardTree());
                    }
                    else
                    {
                        lScene.particles->push_back(new Particle());
                    }
                }
            }
            else if(lSceneChild->getTag() == "media")
            {
                string lDirPath = "";// Tools::GetContentDirectory() + "/Resources/";
                string lType = lSceneChild->getAttributeValue<string>( "type", "video" );
                string lPathFront = lDirPath + lSceneChild->getAttributeValue<string>( "frontImage", "" );
                string lPathBack = lDirPath + lSceneChild->getAttributeValue<string>( "backMedia", "" );
                string lPathAudio = lDirPath + lSceneChild->getAttributeValue<string>( "audioMedia", "" );
                float lDuration = lSceneChild->getAttributeValue<float>( "duration", -1.f );
                float lVolume = lSceneChild->getAttributeValue<float>( "volume", 1.f );
                if(!VideoPlayer::IsMovie(lPathBack))
                {
                    lScene.video = new ImagePlayer(lPathBack, lPathFront, lPathAudio);
                }
                else
                {
                    lScene.video = new VideoPlayer(lPathBack);
                    if(lPathFront.length() > 0)
                    {
                        lScene.video->setFront(lPathFront);
                    }
                }
                
                if(lDuration > 0.f)
                {
                    lScene.video->SetDuration(lDuration);
                }
                
                lScene.video->SetVolume(lVolume);
            }
        }
        scenes.push_back(lScene);
    }
    return;
}

void Set::update()
{

}

void Set::draw()
{

}

SceneData * Set::getScene()
{
    if(scenes.size() <= 0)
    {
        return NULL;
    }
    return &scenes[currentScene];
}

bool Set::IsFirst()
{
    return ((int)currentScene) == 0;
}

void Set::LoadScene(int aScene)
{
    if(scenes.size() <= 0)
    {
        return;
    }
    
    aScene = (aScene == scenes.size() ? 0 : aScene);
    scenes[currentScene].video->stop();
    currentScene = aScene;
    scenes[currentScene].video->play();
    
    if(aScene == 0)
    {
        if(sSoundPlayer)
        {
            sSoundPlayer->stop();
            sSoundPlayer->play();
        }
    }
}

void Set::NextScene()
{
    LoadScene(currentScene + 1);
}