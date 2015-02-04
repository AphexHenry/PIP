//
//  TransitionManager.cpp
//  StereoscopicRendering
//
//  Created by Baptiste Bohelay on 2015-02-04.
//
//

#include "TransitionManager.h"
#include "Set.h"

TransitionType TransitionManager::sTransitionType = TRANSITION_TYPE_NONE;
float TransitionManager::sDuration = 0.7f;

void TransitionManager::draw()
{
    switch(sTransitionType)
    {
        case TRANSITION_TYPE_FADE_TO_BLACK:
            drawFadeToBlack();
            break;
        default:
            break;
    }
}

void TransitionManager::drawFadeToBlack()
{
    float lTransitionDuration = sDuration;
    VideoPlayer * lVideo = Set::getScene()->video;
    float lTimeLeft = lVideo->GetTimeLeft();
    float lTimeSinceStart = lVideo->GetTimeCurrent();
    float lBlack = -1.f;
    if(lTimeLeft < lTransitionDuration)
    {
        lBlack = 1.f - (lTimeLeft / lTransitionDuration);
        ci::app::console() << "out " <<  lBlack << " " <<  lTimeLeft << endl;
    }
    else if (lTimeSinceStart < lTransitionDuration)
    {
        lBlack = (1.f - (lTimeSinceStart / lTransitionDuration));
        ci::app::console() << "in " << lBlack << " " <<  lTimeSinceStart << endl;
    }
    
    if(lBlack > -0.00001f)
    {
        gl::color(ColorA(0.f,0.f,0.f,lBlack));
        gl::drawSolidRect(app::App::get()->getWindowBounds());
        gl::color(Color::white());
    }
}