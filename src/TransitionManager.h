//
//  TransitionManager.h
//  StereoscopicRendering
//
//  Created by Baptiste Bohelay on 2015-02-04.
//
//

#ifndef __StereoscopicRendering__TransitionManager__
#define __StereoscopicRendering__TransitionManager__

#include <stdio.h>

/*
 *  Type of transition.
 */
enum TransitionType
{
    TRANSITION_TYPE_NONE,
    TRANSITION_TYPE_FADE_TO_BLACK
};

class TransitionManager
{
public:
    static void SetTransition(TransitionType aTransition, float aDuration){sTransitionType = aTransition; sDuration = aDuration;};
    static void draw();
    
    static void drawFadeToBlack();
    
private:
        static float sDuration;
        static TransitionType sTransitionType;
};

#endif /* defined(__StereoscopicRendering__TransitionManager__) */
