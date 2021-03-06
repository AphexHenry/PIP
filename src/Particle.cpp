#include "Particle.h"
#include "Subdivision.h"
#include "KinectManager.h"
#include "Set.h"
#include "Shared.h"

bool Particle::sReflection = false;

Particle::Particle():Anchor()
{
    mImmuneTimer = 0.f;
    mSize = 0.f;
    mLifeTime = 0.f;
    Reset();
}

void Particle::Reset()
{
    mLifeTime = Rand::randFloat(0., 7.) * 0.3f * Set::getScene()->particleDuration;
    mPositionLast = GetPositionScene(true);
    int lIndex = (int)Rand::randInt(Shared::sAnchors.size() * 0.99);
    mPosition = Shared::sAnchors.at(lIndex)->mPosition;
    mPosition.x = mPosition.x * 2.f - 1.f;
    mPosition.y = -mPosition.y * 2.f + 1.f;
    float speed = 0.13f;
    mSpeed = Vec3f(Rand::randFloat(-speed, speed), Rand::randFloat(-speed, speed), 0.f);
    SceneData * lScene = Set::getScene();
    mLifeTime = Rand::randFloat(7., 12.) * 0.3f * lScene->particleDuration;
    mLifeTimeInit = mLifeTime;
    mSize = Rand::randFloat(0.5f, 2.f) * 1.9 * lScene->scale;
    
    if(lScene->useKinectColor)
    {
        mColor = Shared::sAnchors.at(lIndex)->mColor;
    }
    else if(!lScene->useVideoColor)
    {
        float colorCoeff = Rand::randFloat(1.f);
        mColor = (colorCoeff * lScene->colorMin + (1.f - colorCoeff) * lScene->colorMax);
        float luminosity = Shared::sAnchors.at(lIndex)->mColor.length();
        mColor *= (0.5f + 0.5f * luminosity);
    }
    else
    {
        float luminosity = Shared::sAnchors.at(lIndex)->mColor.length();
        mColor = Shared::sColorCapture[Rand::randInt(2)] * (0.5f + 0.5f * luminosity);
    }
    
    mColor = mColor * Rand::randFloat(lScene->intensityMin, lScene->intensityMax);
    mColor.a = lScene->opacity;
}

void Particle::updateGrid()
{
    Subdivision::getInstance()->Add(this);
}

void Particle::update(float aTimeInterval)
{
    mLifeTime -= aTimeInterval;
    mImmuneTimer -= aTimeInterval;
    Color lColorA = SensorManager::getInstance()->getColorPixel(Vec2f(mPosition.x, mPosition.y));
    float lDistanceColor = lColorA.distanceSquared((Color)mColor);
    if(lDistanceColor > 0.1)
    {
        mLifeTime *= 0.7f;
    }
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
//        mSpeed += Vec3f(Rand::randFloat(-lExplosionStr, lExplosionStr), Rand::randFloat(-lExplosionStr, lExplosionStr), Rand::randFloat(-lExplosionStr, lExplosionStr));
//    }
//    
    mPosition += mSpeed * aTimeInterval;
}

void Particle::draw(bool aIsLeft)
{
    SceneData * lScene = Set::getScene();

    if(!lScene->reflection && sReflection)
        return;
    
    gl::color(mColor);
    gl::pushModelView();
    if(sReflection)
    {
        // reflection
        gl::translate(Vec3f(0, lScene->reflectionHeight / Set::compression, 0));
        gl::scale( 0.95f, -0.95f, 0.95f );
    }

    Vec3f lPos = GetPositionScene(aIsLeft);
    
    lPos.y = lPos.y / Set::compression;
    float lScale = mSize * Set::particleSize * lScene->scale;
    
    switch (lScene->particleType)
    {
        case PARTICLE_TYPE_SPHERE:
//            lPos.z = 0.5f;
            lPos.z = 0.5f + lPos.z * 0.1f;
            gl::translate(lPos);
            gl::scale( lScale, lScale / Set::compression, 1.f );
            glBegin(GL_POLYGON);
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
            gl::drawBillboard( lPos, Vec2f(1,1), 0, Vec3f(1,0,0), Vec3f(0,1,0) );
        default:
            break;
    }
    
    gl::popModelView();
}

Vec3f Particle::GetPositionScene(bool aIsLeft)
{
    SceneData * lScene = Set::getScene();
//    float lWindowWidth = 1.f;
    float lDistanceSensibility = lScene->distanceSensitivity;
    Vec3f lPos = (mPosition * lScene->movement * Vec3f(1.f / lScene->compressionY, lScene->compressionY, 1.f)) + lScene->position;
    lPos.x += (lScene->position.z + mPosition.z * lDistanceSensibility) * (aIsLeft ? -1.f : 1.f) * 0.01f;
    lPos.z = -0.2f;
    return lPos;
}

