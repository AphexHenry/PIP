#include "Anchor.h"
#include "cinder/Rand.h"
#include "cinder/gl/gl.h"

using namespace ci;

Anchor::Anchor()
{
    mSize = 1.f;
}

void Anchor::draw()
{
//    getWindowSize();
    float lScale = 800;
    float lSize = 10;
    gl::color(1.f, 0.8f, 0.9f);
    gl::drawSolidRect(Rectf(mPosition.x * lScale, mPosition.y * lScale, mPosition.x * lScale + lSize, mPosition.y * lScale + lSize));
    gl::color(1.f, 1.f, 1.f);
}