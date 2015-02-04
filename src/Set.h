/*
 *  Tools.h
 *  SurfaceBasic
 *
 *  Created by TI mini three on 12-03-19.
 *  Copyright 2012 Tangible Interaction Design Inc. All rights reserved.
 *
 */

#ifndef SET_H
#define SET_H

#include <string>
#include <map>
#include "Cinder/Vector.h"
#include "cinder/Utilities.h"
#include "VideoPlayer.h"
#include "SoundPlayer.h"
#include "Particle.h"

enum ParticleType
{
    PARTICLE_TYPE_SPHERE = 0,
    PARTICLE_TYPE_LINE,
    PARTICLE_TYPE_IMG,
    PARTICLE_TYPE_COUNT
};

enum SensorType
{
    SENSOR_TYPE_CAM = 0,
    SENSOR_TYPE_CURSOR,
    SENSOR_TYPE_KINECT
};

struct SceneData
{
    float      scale;
    ci::Vec3f  position;
    ci::Vec3f  movement;
    float  particleDuration;
    ci::Color  colorMin;
    ci::Color  colorMax;
    float      intensityMin;
    float      intensityMax;
    bool       reflection;
    float      reflectionHeight;
    float      eyesSeparation;
    float      opacity;
    bool       useVideoColor;
    bool       useKinectColor;
    ParticleType particleType;
    VideoPlayer *   video;
    vector<Particle * > * particles;
};

class Set
{
public:
    
    static void setup(SensorType aSensorType);
    static void draw();
    static void update();
    static void ToggleHide();
    static SceneData * getScene();
    static void LoadScene(int aScene);
    static void NextScene();
    static bool IsFirst();
    
    static 	float  frictionStr;
    static 	float  gravity;
    static 	float  strengthMove;
    static 	float  frameRate;
    static 	float  heratic;
    static 	float  heratic2;
    static  float  coeffZKinect;
    static 	float  lineVisibility;
    static 	float  particleSize;
    static 	float  particleStrCoeff;
    static  int    bodyTracked;
    static  bool   mirror;
    static  bool   seeSkeleton;
    static  float  compression;
    
    static SoundPlayer * sSoundPlayer;
    
    static std::map<string, ParticleType> particleMapper;
    
protected:
    
    static int     currentScene;
    static std::vector<SceneData> scenes;
    
    static bool hide;
    
};

#endif