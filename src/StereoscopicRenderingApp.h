
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/gl/StereoAutoFocuser.h"
#include "cinder/gl/Vbo.h"

#include "cinder/Camera.h"
#include "cinder/Font.h"
#include "cinder/ImageIo.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Rand.h"
#include "cinder/TriMesh.h"
#include "cinder/Utilities.h"

#include "Resources.h"

#include <boost/format.hpp>

#include "VideoPlayer.h"
#include "Particle.h"
#include "SensorManager.h"

class StereoscopicRenderingApp : public AppBasic {
  public:
	typedef enum { SET_CONVERGENCE, SET_FOCUS, AUTO_FOCUS } FocusMethod;
	typedef enum { MONO, ANAGLYPH_RED_CYAN, SIDE_BY_SIDE, TV_SIDE_2 } RenderMethod;
public:
	void prepareSettings( Settings *settings );

	void setup();	
	void update();
    void UpdateSubdivisions(float aTimeInterval);
	void draw();
    void DrawParticles(Vec2i aSize);

	void keyDown( KeyEvent event );

	void resize();
private:
    
    // Load the scenes videos and all.
    void LoadScenes();
    void drawLoading();
    
    void LoadNextScene();
    void ApplySceneSettings();
    
	void createFbo();

	void renderAnaglyph( const Vec2i &size, const ColorA &left, const ColorA &right );
	void renderSideBySide( const Vec2i &size );

	void render(bool aIsLeft);
	void renderUI();

	bool					mLoaded;
    bool					mScenesLoaded;
	bool					mLoadingDrawn;

	FocusMethod				mFocusMethod;
	RenderMethod			mRenderMethod;

	MayaCamUI				mMayaCam;
	CameraStereo			mCamera;

	gl::StereoAutoFocuser	mAF;

	gl::GlslProg			mShaderPhong;
	gl::GlslProg			mShaderAnaglyph;
	gl::GlslProg			mShaderInterlaced;

    gl::Texture             mLoadingScreen;

	gl::Fbo					mFbo;

	Color					mColorBackground;

	Font					mFont;
    
    VideoPlayer       *      mVideoPlayer;
    
    vector<Particle *> *    mPartNiv1;
    
    float                   mTime;
    
    SensorManager * mSensorManager;
};