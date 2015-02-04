#ifndef _SHARED
#define _SHARED

#include "Anchor.h"
#include <vector>
#include "cinder/Color.h"

class Shared
{
public:
    
    static void setup();
    
    static std::vector<Anchor *> sAnchors;
    
    static cinder::ColorA sColorCapture[2];
};

#endif