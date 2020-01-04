#include "ParticleBillboardV2.h"
#include "Set.h"
#include "cinder/Rand.h"

using namespace ci;

vector<ci::gl::Texture> ParticleBillboardV2::sTextures;

void ParticleBillboardV2::Reset()
{
    mTextureIndex = Rand::randInt(sTextures.size());
    Particle::Reset();
    mSize = Rand::randFloat(0.8f, 1.3f);
    mSizeInit = mSize;
    mRotation = Rand::randFloat(360.f);
    mRotationSpeed = Rand::randFloat(-10.f, 10.f);
}

void ParticleBillboardV2::AddTexture(ci::gl::Texture aTexture)
{
    sTextures.push_back(aTexture);
}

void ParticleBillboardV2::update(float aTimeInterval)
{
    if(mLifeTime <= 0.f)
    {
        Reset();
    }
    mLifeTime -= aTimeInterval;
    float lCoeffLife = mLifeTime / mLifeTimeInit;
    mAlpha = fmin(2.f * sin(lCoeffLife * 3.14), 1.f);
    mRotation += mRotationSpeed * aTimeInterval;
    mSize = mSizeInit * (0.2 + lCoeffLife * 0.8);
    mPosition.y += 2.f * aTimeInterval * 0.03 / (mSize * mLifeTimeInit);
}

/*
 *  Draw particle.
 *  aCoeffLifeColor : normalized age of the particle.
 */
void ParticleBillboardV2::draw(bool aIsLeft)
{
    SceneData * lScene = Set::getScene();

    if(!lScene->reflection && sReflection)
        return;
    
    float lAlpha = sReflection ? mAlpha * lScene->opacity * 0.4f : mAlpha * lScene->opacity;
    gl::color(mColor.r, mColor.g, mColor.b, lAlpha);
    gl::pushMatrices();
    
    if(sReflection)
    {
        // reflection
        gl::translate(Vec3f(0, lScene->reflectionHeight, 0));
        gl::scale( 1.f, -1.f, 1.f );
    }

    Vec3f lPos = GetPositionScene(aIsLeft);
//    lPos.z -= mSize * 0.01;
    
    sTextures[mTextureIndex].enableAndBind();
    
    gl::scale( 1.f, 1.f / Set::compression, 1.f);
    gl::drawBillboard( lPos, Vec2f(Set::particleSize * mSize * lScene->scale, Set::particleSize * mSize * lScene->scale), mRotation, Vec3f(1,0,0), Vec3f(0,1,0) );
    
    sTextures[mTextureIndex].unbind();
    
    gl::popMatrices();
}

