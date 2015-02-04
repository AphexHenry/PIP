#include "Anchor.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"

using namespace ci;

Anchor::Anchor(int aUser) {
    mSize = 1.f;
    if(Rand::randFloat(1.f) > 0.5)
        mPosition = Vec3f(Rand::randFloat(0.1f, 0.1f), Rand::randFloat(0.1f, 0.1f / (float)Tools::getWindowRatio()), 0);
    else
        mPosition = Vec3f(Rand::randFloat(0.3f, 0.3f), Rand::randFloat(0.1f, 0.1f / (float)Tools::getWindowRatio()), 0);
    
    mUser = aUser;
}

//--------------------------------------------------------------
void Anchor::update(){
    
}

void Anchor::drawDebug()
{

}