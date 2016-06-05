
#include "cinder/Rand.h"
#include "CursorManager.h"

using namespace ci;
using namespace ci::app;

bool CursorManager::setup()
{
    mPositionsPart.clear();
    return true;
}

void CursorManager::update(float aTimeInterval)
{
    float lDelta = AppBasic::get()->getFpsSampleInterval();
    ivec2 lPosTemp = AppBasic::getMousePos();
    vec3 lPos;
    lPos.x = lPosTemp.x;
    lPos.y = lPosTemp.y;
    float lSpeed = fmin((lPos - mPositionMouse).lengthSquared() / (lDelta * AppBasic::get()->getWindowWidth()), 2.f);
    mPositionMouse.x = lPos.x;
    mPositionMouse.y = lPos.y;
    
    int lNumPart = fmin((lSpeed * 10), 10);
    vec3 lPositionToAdd;
    for(int i = 0; i < lNumPart; i++)
    {
        console() << lNumPart << endl;
        if(mPositionsPart.size() < 1.5 / aTimeInterval)
        {
            float lLargeness = lSpeed * 0.02f;
            lPositionToAdd = (mPositionMouse / AppBasic::get()->getWindowWidth()) + vec3(Rand::randFloat(-lLargeness, lLargeness), Rand::randFloat(-lLargeness, lLargeness), 0);
            lPositionToAdd.x = 1.f - lPositionToAdd.x;
            mPositionsPart.push_back(lPositionToAdd);
        }
    }
    
    if(mPositionsPart.size() > 0)
    {
        list<vec3>::iterator it = std::next(mPositionsPart.begin(), Rand::randInt(mPositionsPart.size() - 1));
        mPositionsPart.erase(it);
    }
}

void CursorManager::draw()
{

}

void CursorManager::UpdateAnchors()
{
    // draw anchors
    if(mPositionsPart.size() > 0)
    {
        list<vec3>::iterator it = mPositionsPart.begin();
        float gap = (float)mPositionsPart.size() / (float)Shared::sAnchors.size();
        int prev = 0;
        int now;
        for(int i = 0; (i < Shared::sAnchors.size()); i++)
        {
            now = floor(gap * (float)i);
            std::advance(it, now - prev);
            prev = now;
            Shared::sAnchors[i]->SetPosition(*it);
        }
    }
    else
    {
        for(int i = 0; (i < Shared::sAnchors.size()); i++)
        {
            Shared::sAnchors[i]->SetPosition(vec3(0.5f + Rand::randFloat(1.f), 10.f, 0.f));
        }
    }
}
