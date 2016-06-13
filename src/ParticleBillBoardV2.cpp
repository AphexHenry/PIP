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
    
    gl::color(mColor.r, mColor.g, mColor.b, mAlpha * lScene->opacity);
    gl::pushMatrices();
    
    if(sReflection)
    {
        // reflection
        gl::translate(Vec3f(0, lScene->reflectionHeight, 0));
        gl::scale( 0.95f, -.95f, .95f );
    }

    Vec3f lPos = GetPositionScene(aIsLeft);
    lPos.z -= mSize * 0.01;
    
    sTextures[mTextureIndex].enableAndBind();
    
    gl::scale( 1.f, 1.f / Set::compression, 1.f);
    gl::drawBillboard( lPos, Vec2f(Set::particleSize * mSize * lScene->scale, Set::particleSize * mSize * lScene->scale), mRotation, Vec3f(1,0,0), Vec3f(0,1,0) );
    
    sTextures[mTextureIndex].unbind();
    
    gl::popMatrices();
}

