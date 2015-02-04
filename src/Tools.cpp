/*
 *  Tools.cpp
 *  SurfaceBasic
 *
 *  Created by TI mini three on 12-03-19.
 *  Copyright 2012 Tangible Interaction Design Inc. All rights reserved.
 *
 */

#include "Tools.h"
using namespace std;

int Tools::sWidth = 1.f;
int Tools::sHeight = 1.f;
float Tools::sRatio = 1.f;

float Tools::getWindowRatio()
{
    return sRatio;
}

int Tools::getWindowWidth()
{
    return sWidth;
}

int Tools::getWindowHeight()
{
    return sHeight;
}

string Tools::convertInt(int number)
{
    if (number == 0)
        return "0";
    
    string temp="";
    string returnvalue="";
    while (number>0)
    {
        temp+=number%10+48;
        number/=10;
    }
    for (int i=0;i<temp.length();i++)
        returnvalue+=temp[temp.length()-i-1];
    
    return returnvalue;
}