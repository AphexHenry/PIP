#include "Particle.h"
#include "Subdivision.h"
#include "KinectManager.h"
#include "Set.h"
#include "Shared.h"

bool Particle::sReflection = false;

Particle::Particle():Anchor(0)
{
    mImmuneTimer = 0.f;
    Reset();
    mLifeTime = Rand::randFloat(0., 7.) * 0.3f * Set::getScene()->particleDuration;
    mSize = 0.f;
}

void Particle::Reset()
{
    mPositionLast = GetPositionScene();
    mPosition = Shared::sAnchors.at((int)Rand::randInt(Shared::sAnchors.size() * 0.99))->mPosition;
    mPosition.x = mPosition.x * 2.f - 1.f;
    mPosition.y = -mPosition.y * 2.f + 1.f;
    float speed = 0.13f;
    mSpeed = vec3(Rand::randFloat(-speed, speed), Rand::randFloat(-speed, speed), 0.f);
    SceneData * lScene = Set::getScene();
    mLifeTime = Rand::randFloat(7., 12.) * 0.3f * lScene->particleDuration;
    mLifeTimeInit = mLifeTime;
    mSize = Rand::randFloat(0.5f, 2.f) * 1.9 * lScene->scale;
    
    if(!lScene->useVideoColor)
    {
        float colorCoeff = Rand::randFloat(1.f);
        mColorInit = mColor = (colorCoeff * lScene->colorMin + (1.f - colorCoeff) * lScene->colorMax) * Rand::randFloat(lScene->intensityMin, lScene->intensityMax);;
    }
    else
    {
        mColorInit = mColor = Shared::sColorCapture[Rand::randInt(2)] * Rand::randFloat(lScene->intensityMin, lScene->intensityMax);//colorCoeff *
    }
}

void Particle::updateGrid()
{
    Subdivision::getInstance()->Add(this);
}

void Particle::update(float aTimeInterval)
{
    mLifeTime -= aTimeInterval;
    mImmuneTimer -= aTimeInterval;
    if(mLifeTime <= 0.f)
    {
        Reset();
    }
    float lCoeffLife = mLifeTime / mLifeTimeInit;
    mSize =  2.f * fmin(2.f * sin(lCoeffLife * 3.14), 1.f);
    
//    mSpeed *= 0.9;
//
//    mSpeed.y -= Set::gravity * 0.1f * aTimeInterval;
//    
//    float lExplosionStr = 0.4f;
//    if(Set::explosion)
//    {
//        mSpeed += vec3(Rand::randFloat(-lExplosionStr, lExplosionStr), Rand::randFloat(-lExplosionStr, lExplosionStr), Rand::randFloat(-lExplosionStr, lExplosionStr));
//    }
//    
    mPosition += mSpeed * aTimeInterval;
}

void Particle::drawDebug(int aCoeffLifeColor)
{
    SceneData * lScene = Set::getScene();

    if(!lScene->reflection && sReflection)
        return;
    
    gl::color(mColor);
    gl::pushMatrices();
    
    if(sReflection)
    {
        // reflection
        gl::translate(vec3(0, lScene->reflectionHeight, 0));
        gl::scale( 0.95f, -0.95f, 0.95f );
    }

    vec3 lPos = GetPositionScene();
    float lScale = mSize * Set::particleSize * lScene->scale;
    
    switch (lScene->particleType)
    {
        case PARTICLE_TYPE_SPHERE:
            gl::translate(lPos);
//            gl::drawSphere(lPos, mSize * Set::particleSize * lScene->scale, 5);
            gl::scale( lScale, lScale, 1.f );
            gl::begin(GL_POLYGON);
            for(int i = 0; i < 6; ++i) {
                glVertex2d(sin(i/6.0*2*M_PI) + 0.5f * cos(mLifeTime * (1.f + 2.f * mPosition.x) + (mPosition.y * (2 * i + 1))),
                           cos(i/6.0*2*M_PI) + 0.5f * sin(mLifeTime * (1.f + 2.f * mPosition.y) + (mPosition.x * (2 * i + 1))));
            }
            glEnd();
            break;
        case PARTICLE_TYPE_LINE:
            gl::drawLine(mPositionLast, lPos);
            break;
        case PARTICLE_TYPE_IMG:
            gl::drawBillboard( lPos, vec2(1,1), 0, vec3(1,0,0), vec3(0,1,0) );
        default:
            break;
    }
    
    gl::popMatrices();
}

vec3 Particle::GetPositionScene()
{
    SceneData * lScene = Set::getScene();
//    float lWindowWidth = 1.f;
    return (mPosition * lScene->movement) + lScene->position;
//   return vec3((mPosition.x + lTranslate.x + lScene->position.x) * lWindowWidth * lScene->movement.x, (mPosition.y + lTranslate.y + lScene->position.y) * lWindowWidth * lScene->movement.y, (mPosition.z + lScene->position.z) * lWindowWidth * lScene->movement.z);
}

