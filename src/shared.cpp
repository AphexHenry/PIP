
#include "shared.h"

#define NUM_ANCHOR 200

using namespace std;
using namespace ci;

vector<Anchor *> Shared::sAnchors;
ColorA Shared::sColorCapture[2];

void Shared::setup()
{
    for(int j = 0; j < NUM_ANCHOR; j++)
    {
        sAnchors.push_back(new Anchor(0));
    }
}