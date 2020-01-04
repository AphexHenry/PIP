/*
 Copyright (c) 2015, Baptiste Bohelay.
 Based on the steroscopic renderer by Paul Houx.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include "StereoscopicRenderingApp.h"
#include "KinectManager.h"
#include "CaptureManager.h"
#include "CursorManager.h"
#include "TransitionManager.h"
#include "Shared.h"
#include "Subdivision.h"

using namespace ci;
using namespace ci::app;
using namespace std;

StereoscopicRenderingApp::RenderMethod		StereoscopicRenderingApp::sRenderMethod = ANAGLYPH_RED_CYAN;

void StereoscopicRenderingApp::prepareSettings( Settings *settings )
{
    console() << "prepareSettings" << endl;
	settings->setWindowSize(1200, 900);
//    settings->setFullScreen();
	settings->setTitle("Stereoscopic Rendering");

	// allow high frame rates to test performance
	settings->setFrameRate( 60.0f );
}

void StereoscopicRenderingApp::setup()
{
    app::addAssetDirectory	(	app::getAppPath().string() + "/../Assets/"	 );

    mVideoPlayer = nullptr;
    mPartNiv1 = nullptr;
    
	// enable auto-focussing
    mFocusMethod = SET_CONVERGENCE;

	// setup the camera
	mCamera.setEyePoint( Vec3f(0.2f, 1.3f, -11.5f) );
	mCamera.setCenterOfInterestPoint( Vec3f(0.5f, 1.5f, -0.1f) );
	mCamera.setFov( 60.0f );

	mMayaCam.setCurrentCam( mCamera );

	mColorBackground = Color( 0.f, 0.f, 0.f );

	mFont = Font("Verdana", 24.0f);
    
	mLoaded = false;
    mLoadingDrawn = false;
    mScenesLoaded = false;
    
    Shared::setup();
    
    hideCursor();
    
    mLoadingScreen = gl::Texture(loadImage(getResourcePath(LOADING_SCREEN)));
    
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void StereoscopicRenderingApp::LoadScenes()
{
    try {
		// load shader(s)
        //		mShaderPhong = gl::GlslProg( loadAsset("shaders/phong_vert.glsl"), loadAsset("shaders/phong_frag.glsl") );
		mShaderAnaglyph = gl::GlslProg( loadResource( RES_ANAGLYPH_VERT ), loadResource( RES_ANAGLYPH_FRAG ) );
        //		mShaderInterlaced = gl::GlslProg( loadAsset("shaders/interlaced_vert.glsl"), loadAsset("shaders/interlaced_frag.glsl") );
	}
	catch( const std::exception &e ) {
		// something went wrong, display error and quit
		console() << e.what() << std::endl;
		quit();
	}
    
    SensorManager::getInstance()->setup();
    
    mTime = getElapsedSeconds();
    
    SensorManager::getInstance()->Calibrate();
    
    LoadNextScene();
}

void StereoscopicRenderingApp::update()
{
    if(!mLoaded)
    {
        if(mLoadingDrawn)
        {
            if(!mScenesLoaded)
            {
                LoadScenes();
                mScenesLoaded = true;
            }
            else
            {
                if(SensorManager::getInstance()->isReady())
                {
                    mLoaded = true;
                    mTime = getElapsedSeconds();
                }
            }
        }
        
        return;
    }
    
	float	d, f;
	Area	area;

	switch( mFocusMethod )
	{
	case SET_CONVERGENCE:
		// auto-focus by calculating distance to center of interest
		d = (mCamera.getCenterOfInterestPoint() - mCamera.getEyePoint()).length();
		f = math<float>::min( 5.0f, d * 0.5f );

		// The setConvergence() method will not change the eye separation distance, 
		// which may cause the parallax effect to become uncomfortably big. 
		mCamera.setConvergence( f );
//		mCamera.setEyeSeparation( 0.05f );
		break;
	case SET_FOCUS:
		// auto-focus by calculating distance to center of interest
		d = (mCamera.getCenterOfInterestPoint() - mCamera.getEyePoint()).length();
		f = math<float>::min( 5.0f, d * 0.5f );

		// The setConvergence( value, true ) method will automatically calculate a fitting value for the eye separation distance.
		// There is still no guarantee that the parallax effect stays within comfortable levels,
		// because there may be objects very near to the camera compared to the point we are looking at.
		mCamera.setConvergence( f, true );
		break;
	case AUTO_FOCUS:
		// Here, we use the gl::StereoAutoFocuser class to determine the best focal length,
		// based on the contents of the current depth buffer. This is by far the best method of
		// the three, because it guarantees the parallax effect will never be out of bounds.
		// Depending on the rendering method, we can sample different area's of the screen
		// to optimally detect details. This is not required, however.
		// Use the UP and DOWN keys to adjust the intensity of the parallax effect.
		switch( sRenderMethod ) 
		{
		case MONO:
			break;
		case SIDE_BY_SIDE:
			// sample half the left eye, half the right eye
			area = gl::getViewport();
			area.expand( -area.getWidth()/4, 0 );
			mAF.autoFocus( &mCamera, area );
			break;
		case ANAGLYPH_RED_CYAN:
			// sample the depth buffer of one of the FBO's
			mAF.autoFocus( &mCamera, mFbo );
			break;
            default:
                break;
		}
		break;
	}
    
    float lTimeElapsed = getElapsedSeconds() - mTime;
    mTime = getElapsedSeconds();
    
    mVideoPlayer->update(lTimeElapsed);
    
    if(mVideoPlayer->isDone())
    {
        LoadNextScene();
    }

    SensorManager::getInstance()->update(lTimeElapsed);
    SensorManager::getInstance()->UpdateAnchors();
    
    if(mPartNiv1)
    for(int i = 0; i < mPartNiv1->size(); i++)
    {
        mPartNiv1->at(i)->update(lTimeElapsed);
    }
    
    UpdateSubdivisions(lTimeElapsed);
}

void StereoscopicRenderingApp::UpdateSubdivisions(float aTimeInterval)
{
    Subdivision * lSubdivision = Subdivision::getInstance();
    lSubdivision->Update(aTimeInterval);
    lSubdivision->Clear();
    
    for(int i = 0; i < Shared::sAnchors.size(); i++)
    {
        lSubdivision->AddAnchor(Shared::sAnchors[i]);
    }
    
    if(mPartNiv1)
    for(int i = 0; i < mPartNiv1->size(); i++)
    {
        lSubdivision->Add(mPartNiv1->at(i));
    }
}

void StereoscopicRenderingApp::drawLoading()
{
    gl::clear( Color::black() );
    Vec2f lSize = getWindowSize();
    Rectf centeredRect = Rectf( mLoadingScreen.getBounds() ).getCenteredFit( Area(0, 0, lSize.x, lSize.y), true );
    gl::draw( mLoadingScreen, centeredRect  );
    mLoadingDrawn = true;
}
void StereoscopicRenderingApp::draw()
{
    if(!mLoaded)
    {
        drawLoading();
        return;
    }
    
    gl::pushMatrices();
    gl::clear( Color::black() );
    Vec2i canvasSize;
    Vec2i partSize;
    
    if(sRenderMethod == ANAGLYPH_RED_CYAN)
    {
        // bind the FBO and clear its buffer
        mFbo.bindFramebuffer();
        gl::clear( mColorBackground );
        gl::setViewport( mFbo.getBounds() );
        canvasSize = getWindowSize();
        partSize = mFbo.getSize();
    }
    else if(sRenderMethod == TV_SIDE_2)
    {
       canvasSize = mFbo.getSize();
       partSize = getWindowSize();
        gl::scale(0.5, 1.f, 1.f);
    }
    else
    {
        canvasSize = getWindowSize();
        partSize = mFbo.getSize();
    }

    if(mVideoPlayer) {
        mVideoPlayer->draw(canvasSize);
    }
    
    if(sRenderMethod == ANAGLYPH_RED_CYAN)
    {
        gl::setViewport( getWindowBounds() );
    }
    
    Particle::SetReflection(true);
    gl::enableAlphaBlending();
    DrawParticles(partSize);
    
    if(sRenderMethod == ANAGLYPH_RED_CYAN)
    {
        gl::setViewport( mFbo.getBounds() );
    }
    
    if(mVideoPlayer) {
        mVideoPlayer->drawFrontReflection(canvasSize);
    }
        
    if(sRenderMethod == ANAGLYPH_RED_CYAN)
    {
        gl::setViewport( getWindowBounds() );
    }
    
    Particle::SetReflection(false);
    DrawParticles(partSize);
    
    if(sRenderMethod == ANAGLYPH_RED_CYAN)
    {
        gl::setViewport( mFbo.getBounds() );
    }
    
    if(mVideoPlayer) {
    mVideoPlayer->drawFront(canvasSize);
    }
        
    if(sRenderMethod == ANAGLYPH_RED_CYAN)
    {
        // unbind the FBO
        mFbo.unbindFramebuffer();
        gl::setViewport( getWindowBounds() );
        if(sRenderMethod == ANAGLYPH_RED_CYAN)
        {
            // enable the anaglyph shader
            mShaderAnaglyph.bind();
            ColorA left = Color(1, 0, 0);
            ColorA right = Color(0, 1, 1);
            mShaderAnaglyph.uniform( "tex0", 0 );
            mShaderAnaglyph.uniform( "clr_left", left );
            mShaderAnaglyph.uniform( "clr_right", right );
        }
        
        // bind the FBO texture and draw a full screen rectangle,
        // which conveniently is exactly what the following line does
        Vec2i size = getWindowSize();
        Area lTextureBounds = mFbo.getTexture().getBounds();
        lTextureBounds.x2 *= 1.f;
        lTextureBounds.y2 *= 2.f;
        Rectf centeredRect = Rectf( lTextureBounds ).getCenteredFit( Area(0, size.y, size.x, 0), true );
        //        gl::draw( , Rectf(0, float(size.y) * 0.8, float(size.x) * 0.8, 0) );
        centeredRect.y1 = centeredRect.y2;
        centeredRect.y2 = 0;
        centeredRect.y2 = -centeredRect.y1 * 0.5;
        centeredRect.y1 *= 1.5f;
        gl::draw( mFbo.getTexture(), centeredRect );
        
        if(sRenderMethod == ANAGLYPH_RED_CYAN)
        {
            // disable the anaglyph shader
            mShaderAnaglyph.unbind();
        }
    }
    gl::popMatrices();
    
    if(!SensorManager::getInstance()->isCalibrated())
    {
        gl::color(Color::black());
        gl::drawSolidRect(getWindowBounds());
        gl::color(Color::white());
        gl::drawString("Calibrating", getWindowCenter());
    }
    
    TransitionManager::draw();
}


void StereoscopicRenderingApp::DrawParticles(Vec2i aSize)
{
    // stereoscopic rendering
	switch( sRenderMethod )
	{
        case MONO:
            // render mono camera
            mCamera.disableStereo();
            render();
            break;
        case TV_SIDE_2:
        case ANAGLYPH_RED_CYAN:
            renderSideBySide( aSize );
            break;
        case SIDE_BY_SIDE:
            renderSideBySide( Vec2i(aSize.x / 2, aSize.y) );
            break;
	}
}

void StereoscopicRenderingApp::keyDown( KeyEvent event )
{
	switch( event.getCode() )
	{
#ifndef MY_APP
    case KeyEvent::KEY_c:
        SensorManager::getInstance()->Calibrate();
        break;
#endif
	case KeyEvent::KEY_ESCAPE:
		quit();
		break;
	case KeyEvent::KEY_UP:
            mCamera.setEyeSeparation( mCamera.getEyeSeparation() - 0.003);
            cout << "mCamera.getEyeSeparation() " << mCamera.getEyeSeparation() << endl;
		break;
	case KeyEvent::KEY_DOWN:
            mCamera.setEyeSeparation( mCamera.getEyeSeparation() + 0.003 );
            cout << "mCamera.getEyeSeparation() " << mCamera.getEyeSeparation() << endl;
		break;
	case KeyEvent::KEY_F2:
		sRenderMethod = ANAGLYPH_RED_CYAN;
		createFbo();
		break;
	case KeyEvent::KEY_F3:
		sRenderMethod = SIDE_BY_SIDE;
		createFbo();
		break;
    case KeyEvent::KEY_F4:
        sRenderMethod = TV_SIDE_2;
        createFbo();
        break;
    case KeyEvent::KEY_RIGHT:
    case KeyEvent::KEY_n:
        LoadNextScene();
        break;
	}
}

/* 
 *  Load next scene and apply all its settings.
 */
void StereoscopicRenderingApp::LoadNextScene()
{
    Set::NextScene();
    ApplySceneSettings();
    
}

void StereoscopicRenderingApp::ApplySceneSettings()
{
    SceneData * lScene = Set::getScene();
    if(lScene)
    {
        mVideoPlayer = lScene->video;
        mPartNiv1 = lScene->particles;
        mCamera.setEyeSeparation( lScene->eyesSeparation );
    }
}

void StereoscopicRenderingApp::resize()
{
	// make sure the camera's aspect ratio remains correct
	mCamera.setAspectRatio( getWindowAspectRatio() );
	mMayaCam.setCurrentCam( mCamera );

	// create/resize the Frame Buffer Object required for some of the render methods
	createFbo();
}

void StereoscopicRenderingApp::createFbo()
{
	Vec2i size = getWindowSize();

	gl::Fbo::Format fmt;
	fmt.setMagFilter( GL_LINEAR );
	fmt.setSamples(16);
	fmt.setCoverageSamples(16);

    Set::compression = 1.f;
    
	switch( sRenderMethod )
	{
    case TV_SIDE_2:
        Set::compression = 0.5f;
        mFbo = gl::Fbo( size.x * 2.f, size.y, fmt );
        break;
	case ANAGLYPH_RED_CYAN: 
		// by doubling the horizontal resolution, we can effectively render
		// both the left and right eye views side by side at full resolution
		mFbo = gl::Fbo( size.x * 2.f, size.y, fmt );
		break;
	default:
		mFbo = gl::Fbo( size.x * 2.f, size.y, fmt );
		break;
	}
}

void StereoscopicRenderingApp::renderAnaglyph(  const Vec2i &size, const ColorA &left, const ColorA &right )
{	
	// bind the FBO and clear its buffer
//	mFbo.bindFramebuffer();
//	gl::clear( mColorBackground );

	// render the scene using the side-by-side technique
	renderSideBySide( mFbo.getSize() );

	// unbind the FBO
	mFbo.unbindFramebuffer();

	// enable the anaglyph shader
	mShaderAnaglyph.bind();
	mShaderAnaglyph.uniform( "tex0", 0 );
//	mShaderAnaglyph.uniform( "clr_left", left );
//	mShaderAnaglyph.uniform( "clr_right", right );	

	// bind the FBO texture and draw a full screen rectangle,
	// which conveniently is exactly what the following line does
//	gl::draw( mFbo.getTexture(), Rectf(0, float(size.y), float(size.x), 0) );

//	// disable the anaglyph shader
//	mShaderAnaglyph.unbind();
}

void StereoscopicRenderingApp::renderSideBySide( const Vec2i &size )
{	
	// store current viewport
	glPushAttrib( GL_VIEWPORT_BIT );

	// draw to left half of window only
	gl::setViewport( Area(0, 0, size.x / 2, size.y) );

	// render left camera
	mCamera.enableStereoLeft();
	render();

	// draw to right half of window only
	gl::setViewport( Area(size.x / 2, 0, size.x, size.y) );

	// render right camera
	mCamera.enableStereoRight();
	render();

	// restore viewport
	glPopAttrib();
}

void StereoscopicRenderingApp::render()
{
    // setup the camera
	mCamera.setEyePoint( Vec3f(0.f, 0.f, -1.f ));
	mCamera.setCenterOfInterestPoint( Vec3f(0.f, 0.f, 0.f) );
    
	// enable 3D rendering
//	gl::enableDepthRead();
//	gl::enableDepthWrite();

	// set 3D camera matrices
	gl::pushMatrices();
	gl::setMatrices( mCamera );

    // enable phong shading
//    mShaderPhong.bind();	
    
    // draw part
    if(mPartNiv1)
    {
        for(int i = 0; i < mPartNiv1->size(); i++)
        {
            mPartNiv1->at(i)->draw(0);
        }
//        gl::enableAdditiveBlending();
//        for(int i = 0; i < mPartNiv1->size(); i++)
//        {
//            mPartNiv1->at(i)->draw(0);
//        }
//        gl::disableAlphaBlending();
    }
//    gl::enableAlphaBlending();
//    mShaderPhong.unbind();

	// draw floor
//	gl::enableAlphaBlending();
//	gl::color( ColorA(1,1,1,0.75f) );
//	gl::drawCube( Vec3f(0.0f, -0.5f, 0.0f), Vec3f(200.0f, 1.0f, 200.0f) );
//	gl::disableAlphaBlending();

	// restore 2D rendering
	gl::popMatrices();
//	gl::disableDepthWrite();
//	gl::disableDepthRead();
    gl::color( Color(1.f, 1.f, 1.f) );
//
//	// render UI
//	if( mDrawUI ) renderUI();
}

CINDER_APP_BASIC( StereoscopicRenderingApp, RendererGl )