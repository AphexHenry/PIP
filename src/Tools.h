/*
 *  Tools.h
 *  SurfaceBasic
 *
 *  Created by TI mini three on 12-03-19.
 *  Copyright 2012 Tangible Interaction Design Inc. All rights reserved.
 *
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <string>
#include "Cinder/Vector.h"
#include "Cinder/Color.h"
#include "cinder/app/AppBasic.h"

class Tools
{
public:
    
    static int getWindowWidth();
    
    static float getWindowRatio();

    static int getWindowHeight();
    
    static void setWindowSize(int aWidth, int aHeight){sWidth = aWidth; sHeight = aHeight; sRatio = (float)aWidth / (float)aHeight;};
    
    static ci::Color stringToColor(std::string aInput);
    
    static std::string convertInt(int number);
    static float GetFloat(std::string aString);
    
    static std::string GetOutResourcePath();
    static std::string GetOutResourcePath(cinder::fs::path aAsset);
    static std::string GetOutResourcePath(std::string aAsset);
    static std::string GetContentDirectory();
private:
    static int sWidth;
    static int sHeight;
    static float sRatio;
    
};

#endif