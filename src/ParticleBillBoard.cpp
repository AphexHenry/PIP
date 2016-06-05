#include "ParticleBillBoard.h"
#include "Set.h"

using namespace ci;

ci::gl::Texture ParticleBillboard::sTexture;

void ParticleBillboard::SetTexture(ci::gl::Texture aTexture)
{
    sTexture = aTexture;
}

void ParticleBillboard::drawDebug(int aCoeffLifeColor)
{
    SceneData * lScene = Set::getScene();

    if(!lScene->reflection && sReflection)
        return;
    
    gl::color(mColor.r, mColor.g, mColor.b);
    gl::pushMatrices();
    
    if(sReflection)
    {
        // reflection
        gl::scale( 0.9f, -.9f, .9f );
    }

    vec3 lPos = GetPositionScene();
    lPos.z -= mSize * 0.01;
    
    if(sTexture)
    {
        sTexture.enableAndBind();
    }
    
//    gl::translate(lPos);
//    gl::scale(vec2(Set::particleSize * mSize * lScene->scale, Set::particleSize * mSize * lScene->scale));
//    
//    glBegin(GL_QUADS);
//    // Front Face
//    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);  // Bottom Left Of The Texture and Quad
//    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);  // Bottom Right Of The Texture and Quad
//    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);  // Top Right Of The Texture and Quad
//    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);  // Top Left Of The Texture and Quad

//    glEnd();
    
    gl::drawBillboard( lPos, vec2(Set::particleSize * mSize * lScene->scale, Set::particleSize * mSize * lScene->scale), mSize, vec3(1,0,0), vec3(0,1,0) );
    
    if(sTexture)
    {
        sTexture.unbind();
    }
    
    gl::popMatrices();
}

