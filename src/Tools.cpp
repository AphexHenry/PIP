/*
 *  Tools.cpp
 *  SurfaceBasic
 *
 *  Created by TI mini three on 12-03-19.
 *  Copyright 2012 Tangible Interaction Design Inc. All rights reserved.
 *
 */

#include "Tools.h"
#include <vector>
 #include <boost/algorithm/string.hpp>
#include <sstream>

using namespace std;
using namespace ci;

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

Color Tools::stringToColor(std::string aInput)
{
    std::vector<std::string> vec;
    boost::split(vec, aInput, boost::is_any_of(","));
    for(int i = 0; i < vec.size(); i++)
    {
        boost::trim(vec[i]);
    }
    Color lReturnVec(GetFloat(vec[0]), GetFloat(vec[1]), GetFloat(vec[2]));
    return lReturnVec;
}

float Tools::GetFloat(string aString)
{
    std::string value = aString;
    boost::trim(value);
    
    float lReturn;
    istringstream buffer(value);
    buffer >> lReturn;
    return lReturn;
}

string Tools::GetContentDirectory()
{
    return app::getAppPath().string() + "/Contents";
}

string Tools::GetOutResourcePath()
{
    return app::getAppPath().string() + "/../Assets/";
}

string Tools::GetOutResourcePath(fs::path aAsset)
{
    return app::getAppPath().string() + "/../Assets/" + aAsset.string();
}

string Tools::GetOutResourcePath(string aAsset)
{
    return app::getAppPath().string() + "/../Assets/" + aAsset;
}