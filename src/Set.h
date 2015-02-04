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
    bool       useVideoColor;
    bool       useKinectColor;
    ParticleType particleType;
    VideoPlayer *   video;
    vector<Particle * > * particles;
};

enum SceneType
{
    SCENE_TUTO  = -2,
    SCENE_INTRO = -1,
#ifndef STILL_IMAGES_ONLY
    SCENE_WATER = 0,
#endif
    SCENE_CASSE,
    SCENE_STILL_WATER,
    SCENE_CASSE_2,
    SCENE_ROCHER_AIGUE,
    SCENE_PLAINE,
    SCENE_ROCK,
    SCENE_FIELD_TREES,
    SCENE_ROCK_2,
    SCENE_BOULEAUX,
#ifndef STILL_IMAGES_ONLY
    SCENE_MONTAIN_FOG,
#endif
    SCENE_COUNT
};

class Set
{
public:
    
    static void setup(SensorType aSensorType);
    static void draw();
    static void update();
    static void ToggleHide();
    static SceneData * getScene();
    static void LoadScene(SceneType aScene);
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
    static  float   compression;
    
protected:
    
    static SceneType     currentScene;
    static std::map<SceneType, SceneData> scenes;
    
    static bool hide;
    
};

#endif