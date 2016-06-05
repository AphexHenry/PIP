#include "ParticleBillboardTree.h"
#include "Set.h"
#include "cinder/Rand.h"

using namespace ci;

vector<ci::gl::Texture> ParticleBillboardTree::sTextures;

void ParticleBillboardTree::Reset()
{
    mTextureIndex = Rand::randInt(sTextures.size());
    Particle::Reset();
    mSize = Rand::randFloat(0.8f, 1.3f);
    mRotation = Rand::randFloat(360.f);
    mRotationSpeed = Rand::randFloat(-10.f, 10.f);
}

void ParticleBillboardTree::AddTexture(ci::gl::Texture aTexture)
{
    sTextures.push_back(aTexture);
}

void ParticleBillboardTree::update(float aTimeInterval)
{
    if(mLifeTime <= 0.f)
    {
        Reset();
    }
    mLifeTime -= aTimeInterval;
    float lCoeffLife = mLifeTime / mLifeTimeInit;
    mAlpha = fmin(2.f * sin(lCoeffLife * 3.14), 1.f);
    mRotation += mRotationSpeed * aTimeInterval;
}

void ParticleBillboardTree::drawDebug(int aCoeffLifeColor)
{
    SceneData * lScene = Set::getScene();

    if(!lScene->reflection && sReflection)
        return;
    
    gl::color(mColor.r, mColor.g, mColor.b, mAlpha);
    gl::pushMatrices();
    
    if(sReflection)
    {
        // reflection
        gl::translate(vec3(0, lScene->reflectionHeight, 0));
        gl::scale( 0.95f, -.95f, .95f );
    }

    vec3 lPos = GetPositionScene();
    lPos.z -= mSize * 0.01;
    
    // doute
//    sTextures[mTextureIndex].enableAndBind();
        sTextures[mTextureIndex].bind();

    
    gl::drawBillboard( lPos, vec2(Set::particleSize * mSize * lScene->scale, Set::particleSize * mSize * lScene->scale), mRotation, vec3(1,0,0), vec3(0,1,0) );
    
    sTextures[mTextureIndex].unbind();
    
    gl::popMatrices();
}

