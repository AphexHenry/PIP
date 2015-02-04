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
#include "ParticleBillBoard.h"
#include "ParticleBillBoardTree.h"
#include "ImagePlayer.h"

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
float Set::coeffZKinect = -1.f / 250.f;
bool  Set::mirror;
float  Set::compression = 1.f;

SceneType     Set::currentScene = SCENE_INTRO;
map<SceneType, SceneData> Set::scenes;

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
    
    SceneData lScene;
#ifdef MY_APP
    lScene.position     = Vec3f(0, 0.1f, 4.5f);
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.5f;
    lScene.eyesSeparation  = 0.009;//good one.
#else
    lScene.position     = Vec3f(0, 0.1f, 4.5f);
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.5f;
    lScene.eyesSeparation  = 0.00105;
#endif

#ifndef STILL_IMAGES_ONLY
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    lScene.scale = 13.6f;
    lScene.video        = new VideoPlayer(FILM_1, IMAGE_1);
    lScene.reflection   = true;
    lScene.reflectionHeight = -0.1f;
    lScene.colorMin        = Color(0.6,0.6f,0.5f);
    lScene.colorMax        = Color(0.9,0.3f,0.3);
    lScene.intensityMin = 0.7f;
    lScene.intensityMax = 0.9f;
    lScene.useKinectColor = false;
    lScene.useVideoColor= true;
    lScene.particleDuration = 0.12f;
    
    lScene.particles = new vector<Particle *>();
    for(int i = 0; i < 600; i++)
    {
        lScene.particles->push_back(new ParticleBillboardTree());
    }

    scenes[SCENE_WATER] = lScene;
#endif

    lScene.particles = new vector<Particle *>();
    lScene.scale = 2.5f;
    lScene.position     = Vec3f(0.2, 0.f, 0.5f);
    lScene.movement     = Vec3f(2.f, 1.f, 1.f) * 0.28f;
    switch(aSensorType)
    {
        case SENSOR_TYPE_CAM:
            lScene.video        = new ImagePlayer(TUTO_SCREEN_CAM_BACK, TUTO_SCREEN_CAM_FRONT, 15.f);
            break;
        case SENSOR_TYPE_CURSOR:
            lScene.video        = new ImagePlayer(TUTO_SCREEN_CURSOR_BACK, TUTO_SCREEN_CURSOR_FRONT, 15.f);
            break;
        case SENSOR_TYPE_KINECT:
            lScene.video        = new ImagePlayer(TUTO_SCREEN_KINECT, TUTO_SCREEN_KINECT, 20.f);
            break;
    }
    lScene.reflection   = false;
    lScene.colorMin        = Color(1,1.,1);
    lScene.colorMax        = Color(0.1f,0.1,1.);
    lScene.useVideoColor= false;
    lScene.particleDuration = 0.26f;
    lScene.intensityMin = 0.f;
    lScene.intensityMax = 1.f;
    lScene.eyesSeparation  = -0.00399999;
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    lScene.useKinectColor = false;
    
    lScene.particles = new vector<Particle *>();
    for(int i = 0; i < 400; i++)
    {
        lScene.particles->push_back(new Particle());
    }
    
    scenes[SCENE_TUTO] = lScene;
    
    lScene.particles = new vector<Particle *>();
    lScene.scale = 3.3f;
    lScene.position     = Vec3f(0.6, 0.f, 0.5f);
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.29f;
    lScene.video        = new ImagePlayer(IMAGE_BACK_3, IMAGE_FRONT_INTRO, AUDIO_2, 30.f);
    lScene.reflection   = false;
    lScene.colorMin        = Color(0,0,0);
    lScene.colorMax        = Color(0.1,0.1,0.2);
    lScene.useVideoColor= true;
    lScene.particleDuration = 0.25f;
    lScene.intensityMin = 0.2f;
    lScene.intensityMax = 0.7f;
    lScene.eyesSeparation  = 0.003;
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    lScene.useKinectColor = false;
    
    lScene.particles = new vector<Particle *>();
    for(int i = 0; i < 300; i++)
    {
        lScene.particles->push_back(new Particle());
    }
    
    scenes[SCENE_INTRO] = lScene;
    
    lScene.particles = new vector<Particle *>();
    lScene.scale        = .4f;
    lScene.position     = Vec3f(0.5,0.005f, 0.5f);
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.07f;
//#ifdef STILL_IMAGES_ONLY
    lScene.video        = new ImagePlayer(IMAGE_2, IMAGE_2_FRONT, AUDIO_6, 40.f);
//#else
//    lScene.video        = new VideoPlayer(FILM_2);
////    lScene.video->setFront(IMAGE_2_FRONT);
//#endif
    lScene.reflection   = false;
    lScene.particleDuration = 0.35f;
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    lScene.intensityMin = 0.1f;
    lScene.intensityMax = 0.9f;
    lScene.useVideoColor= true;
    lScene.useKinectColor = false;
    lScene.colorMin        = Color(0.62, 0.05, 0.05);
    lScene.colorMax        = Color(0.0f, 0.1, 0.0f);
    
#ifdef MY_APP
    lScene.eyesSeparation = 0.024;
#else
    lScene.eyesSeparation  = 0.024;
#endif
    
    for(int i = 0; i < 620; i++)
    {
        lScene.particles->push_back(new Particle());
    }

    scenes[SCENE_PLAINE] = lScene;
    
    lScene.particles = new vector<Particle *>();
    lScene.scale        = 7.f;
    lScene.position     = Vec3f(-1.0, -0.15f, 2.5f);
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.23f;
#ifdef STILL_IMAGES_ONLY
lScene.video        = new ImagePlayer(IMAGE_BACK_6, IMAGE_FRONT_6, AUDIO_2, 40.f);
#else
lScene.video        = new VideoPlayer(FILM_BACK_6);
//lScene.video->setFront(IMAGE_FRONT_6);
#endif
    lScene.reflection   = false;
    lScene.particleDuration = 0.15f;
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    lScene.intensityMin = 1.f;
    lScene.intensityMax = 3.f;
    lScene.useVideoColor= false;
    lScene.useKinectColor = false;
    lScene.colorMin        = Color(1., 0.6, 0.7);
    lScene.colorMax        = Color(1.f, 1., 1.f);
#ifdef MY_APP
    lScene.eyesSeparation  = -0.009;//good one.
#else
    lScene.eyesSeparation  = -0.015;
#endif

#ifndef STILL_IMAGES_ONLY
    for(int i = 0; i < 620; i++)
    {
        lScene.particles->push_back(new ParticleBillboardTree());
    }
    
    scenes[SCENE_MONTAIN_FOG] = lScene;
#endif
    
    lScene.particles = new vector<Particle *>();
    lScene.scale = 21.3f;
    lScene.position = Vec3f(0.6, -0.1, 0.5);
    lScene.video        = new ImagePlayer(IMAGE_BACK_4, IMAGE_FRONT_4, AUDIO_INDU_1, 50.f);
    lScene.video->SetVolume(0.1f);
    lScene.reflection   = false;
    lScene.colorMin        = Color(0,0,0);
    lScene.colorMax        = Color(0.1,0.1,0.2);
    lScene.useVideoColor= true;
    lScene.particleDuration = 0.25f;
    lScene.intensityMin = 0.2f;
    lScene.intensityMax = 0.7f;
    lScene.useKinectColor = false;
#ifdef MY_APP
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.34f;
    lScene.eyesSeparation  = 0.039;
#else
    lScene.movement     = Vec3f(3.f, 1.f, 1.f) * 0.28f;
    lScene.eyesSeparation  = 0.039;
#endif
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    
    lScene.particles = new vector<Particle *>();
    for(int i = 0; i < 100; i++)
    {
        lScene.particles->push_back(new ParticleBillboardTree());
    }
    scenes[SCENE_CASSE] = lScene;
    
    lScene.particles = new vector<Particle *>();

#ifdef MY_APP
    lScene.scale = 3.f;
    lScene.position     = Vec3f(0.6, -0.1f, 0.5f);
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.28f;
#else
    lScene.scale = 2.6f;
    lScene.position     = Vec3f(0.8, -0.4f, 0.5f);
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.68f;
#endif
    lScene.video        = new ImagePlayer(IMAGE_BACK_7, IMAGE_FRONT_7, AUDIO_INDU_1, 50.f);
    lScene.video->SetVolume(0.1f);
    lScene.reflection   = false;
    lScene.colorMin        = Color(0,0,0);
    lScene.colorMax        = Color(0.1,0.1,0.2);
    lScene.useKinectColor = true;
    lScene.useVideoColor= false;
    lScene.particleDuration = 0.15f;
    lScene.intensityMin = 1.f;
    lScene.intensityMax = 0.7f;
#ifdef MY_APP
    lScene.eyesSeparation  = -0.018;
#else
    lScene.eyesSeparation  = 0.051;
#endif
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    
    lScene.particles = new vector<Particle *>();
    for(int i = 0; i < 400; i++)
    {
        lScene.particles->push_back(new Particle());
    }
    
    scenes[SCENE_CASSE_2] = lScene;
    
    lScene.particles = new vector<Particle *>();
    lScene.scale = 3.3f;
#ifdef MY_APP
    lScene.position     = Vec3f(0.4, -0.1f, 0.5f);
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.22f;
#else
    lScene.position     = Vec3f(0.2, -0.1f, 0.5f);
    lScene.movement     = Vec3f(2.f, 1.f, 1.f) * 0.22f;
#endif
    lScene.video        = new ImagePlayer(IMAGE_BACK_3, IMAGE_FRONT_3, AUDIO_ORAGE, 50.f);
    lScene.reflection   = false;
    lScene.colorMin        = Color(0,0,0);
    lScene.colorMax        = Color(0.1,0.1,0.2);
    lScene.useKinectColor = true;
    lScene.useVideoColor= false;
    lScene.particleDuration = 0.25f;
    lScene.intensityMin = 0.6f;
    lScene.intensityMax = 0.7f;
    lScene.eyesSeparation  = 0.00;
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    
    lScene.particles = new vector<Particle *>();
    for(int i = 0; i < 300; i++)
    {
        lScene.particles->push_back(new Particle());
    }
    
    scenes[SCENE_ROCK] = lScene;
    
    lScene.particles = new vector<Particle *>();
    lScene.scale = 5.6f;
#ifdef MY_APP
    lScene.position     = Vec3f(-0.2, -0.1f, 0.5f);
    lScene.eyesSeparation  = -0.021;
#else
    lScene.eyesSeparation  =  -0.024;
    lScene.position     = Vec3f(-0.5, -0.1f, 0.5f);
#endif
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.18f;
#ifdef STILL_IMAGES_ONLY
    lScene.video        = new ImagePlayer(IMAGE_BACK_5, IMAGE_FRONT_5, AUDIO_2, 50.f);
#else
    lScene.video        = new VideoPlayer(FILM_BACK_5);
    lScene.video->setFront(IMAGE_FRONT_5);
#endif
    lScene.reflection   = false;
    lScene.colorMin     = Color(0.86,0.61,0.07);
    lScene.colorMax     = Color(96.f/255.f,181.f/255.f,98.f/255.f);
    lScene.useVideoColor= false;
    lScene.useKinectColor = false;
    lScene.particleDuration = .15f;
    lScene.intensityMin = 0.5f;
    lScene.intensityMax = 0.9f;
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    
    lScene.particles = new vector<Particle *>();
    for(int i = 0; i < 500; i++)
    {
        lScene.particles->push_back(new ParticleBillboardTree());
    }
    
    scenes[SCENE_ROCHER_AIGUE] = lScene;
    
    lScene.particles = new vector<Particle *>();
    lScene.scale = 4.3f;
#ifdef MY_APP
    lScene.position     = Vec3f(0.4, -.1f, 0.5f);
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.2f;
    lScene.eyesSeparation  = 0.003;
#else
    lScene.position     = Vec3f(0.5f, -0.2f, 0.5f);
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.15f;
    lScene.eyesSeparation  = 0.009;
#endif
    
#ifdef STILL_IMAGES_ONLY
    lScene.video        = new ImagePlayer(IMAGE_BACK_8, IMAGE_FRONT_8, AUDIO_3, 50.f);
#else
    lScene.video        = new VideoPlayer(FILM_BACK_8);
    lScene.video->setFront(IMAGE_FRONT_8);
#endif
    lScene.reflection   = false;
    lScene.colorMin     = Color(0,0,0);
    lScene.colorMax     = Color(0.1,0.1,0.2);
    lScene.useVideoColor= false;
    lScene.useKinectColor = true;
    lScene.particleDuration = 0.25f;
    lScene.intensityMin = 0.2f;
    lScene.intensityMax = 0.6f;
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    
    lScene.particles = new vector<Particle *>();
    for(int i = 0; i < 500; i++)
    {
        lScene.particles->push_back(new ParticleBillboardTree());
    }
    
    scenes[SCENE_FIELD_TREES] = lScene;
    
    lScene.particles = new vector<Particle *>();
    lScene.scale = 3.f;
#ifdef MY_APP
    lScene.position     = Vec3f(0., -0.2f, 0.5f);
    lScene.eyesSeparation  = -0.036f;
    lScene.movement     = Vec3f(2.f, 1.f, 1.f) * 0.25f;
#else
    lScene.position     = Vec3f(-0.4, -0.1f, 0.5f);
    lScene.eyesSeparation  = -0.051f;
    lScene.movement     = Vec3f(3.f, 1.f, 1.f) * 0.22f;
#endif
    lScene.video        = new ImagePlayer(IMAGE_BACK_9, IMAGE_FRONT_9, AUDIO_4, 52.f);
    lScene.reflection   = false;
    lScene.colorMin        = Color(0,0,0);
    lScene.colorMax        = Color(0.1,0.1,0.2);
    lScene.useVideoColor= false;
    lScene.useKinectColor = true;
    lScene.particleDuration = 0.25f;
    lScene.intensityMin = 0.4f;
    lScene.intensityMax = 0.8f;
//    lScene.eyesSeparation  = -0.015f;
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    
    lScene.particles = new vector<Particle *>();
    for(int i = 0; i < 300; i++)
    {
        lScene.particles->push_back(new Particle());
    }
    
    scenes[SCENE_ROCK_2] = lScene;
    
    lScene.particles = new vector<Particle *>();
    lScene.scale = .3f;
    lScene.position     = Vec3f(-0.25, 0.1f, 0.5f);
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.07f;
#ifdef STILL_IMAGES_ONLY
    lScene.video        = new ImagePlayer(IMAGE_BACK_10, IMAGE_FRONT_10, AUDIO_4, 52.f);
#else
    lScene.video        = new VideoPlayer(VIDEO_BACK_10);
    lScene.video->setFront(IMAGE_FRONT_10);
#endif
    lScene.reflection   = true;
    lScene.reflectionHeight = 0.1f;
    lScene.colorMin        = Color(0,0,0);
    lScene.colorMax        = Color(0.1,0.1,0.2);
    lScene.useVideoColor= true;
    lScene.useKinectColor = false;
    lScene.particleDuration = 0.25f;
    lScene.intensityMin = 0.2f;
    lScene.intensityMax = 0.7f;
    lScene.eyesSeparation  = -0.009f;
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    
    lScene.particles = new vector<Particle *>();
    for(int i = 0; i < 300; i++)
    {
        lScene.particles->push_back(new Particle());
    }
    
    scenes[SCENE_STILL_WATER] = lScene;
    
    lScene.particles = new vector<Particle *>();
    lScene.scale = 15.3f;
#ifdef MY_APP
    lScene.position     = Vec3f(0., 0.4f, 0.5f);
    lScene.eyesSeparation  = -0.036f;
    lScene.movement     = Vec3f(2.f, 1.f, 1.f) * 0.25f;
#else
    lScene.position     = Vec3f(0., -0.4f, 0.5f);
    lScene.eyesSeparation  = 0.048;
    lScene.movement     = Vec3f(1.f, 1.f, 1.f) * 0.62f;
#endif
    lScene.video        = new ImagePlayer(IMAGE_BACK_11, IMAGE_FRONT_11, AUDIO_4, 52.f);
    lScene.reflection   = false;
    lScene.colorMin        = Color(0.8,0.8f,0.5f);
    lScene.colorMax        = Color(1.,0.1,.2);
    lScene.useVideoColor= false;
    lScene.useKinectColor = false;
    lScene.particleDuration = 0.25f;
    lScene.intensityMin = 1.f;
    lScene.intensityMax = 1.2f;
    lScene.particleType = PARTICLE_TYPE_SPHERE;
    
    lScene.particles = new vector<Particle *>();
    for(int i = 0; i < 500; i++)
    {
        lScene.particles->push_back(new ParticleBillboardTree());
    }
    
    scenes[SCENE_BOULEAUX] = lScene;
}

void Set::update()
{
//    OSCManager * lOSCManager = OSCManager::getInstance();
//    if(Set::explosion)
//    {
//        Set::explosion = false;
//    }
//    else
//    {
//        float lValue = false;
//        lOSCManager->GetValueForAdress("/impulse", lValue);
//        Set::explosion = lValue > 0.1f;
//    }
//    
//    CheckOSC(partLifeTime);
//    CheckOSC(strengthMove);
//    CheckOSC(backgroundWhite);
//    CheckOSC(particlesDark);
//    CheckOSC(heratic);
//    CheckOSC(heratic2);
//    CheckOSC(lineVisibility);
//    CheckOSC(particleSize);
//    CheckOSC(particleVisibility);
//    CheckOSC(particleStrCoeff);
}

//void Set::CheckOSC(float lParameter)
//{
//    float param = -10.f;
//    OSCManager * lOSCManager = OSCManager::getInstance();
//    
//    lOSCManager->GetValueForAdress("/" + lParameter.getName(), param);
//    if(param >= -9.f)
//    {
//        lParameter = param;
//    }
//}

void Set::draw()
{
//    if( !hide )
//    {
//		gui.draw();
//	}
}

SceneData * Set::getScene()
{
    return &scenes[currentScene];
}

bool Set::IsFirst()
{
    return ((int)currentScene) == 0;
}

void Set::LoadScene(SceneType aScene)
{
    aScene = (SceneType)((aScene == SCENE_COUNT) ? 0 : aScene);
    scenes[currentScene].video->stop();
    currentScene = aScene;
    scenes[currentScene].video->play();
}

void Set::NextScene()
{
    LoadScene((SceneType)(currentScene + 1));
}