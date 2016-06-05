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

class Tools
{
public:
    
    static int getWindowWidth();
    
    static float getWindowRatio();

    static int getWindowHeight();
    
    static void setWindowSize(int aWidth, int aHeight){sWidth = aWidth; sHeight = aHeight; sRatio = (float)aWidth / (float)aHeight;};
    
    static std::string convertInt(int number);
    
private:
    static int sWidth;
    static int sHeight;
    static float sRatio;
    
};

#endif