#include "cinder/app/AppBasic.h"
#include "cinder/Perlin.h"
#include "cinder/ImageIo.h"
#include "Resources.h"

#include "KinectManager.h"
#include "CaptureManager.h"

#include "Shared.h"

using namespace ci;
using namespace ci::app;

class HodginParticlesApp : public AppBasic {
 public:	
	Renderer* prepareRenderer() { return new RendererGl( RendererGl::AA_MSAA_2 ); }
	void prepareSettings( Settings *settings );
	void setup();
	
	void update();
    void UpdateSubdivisions(float aTimeInterval);
	void draw();
    
    void keyDown( KeyEvent event );
    
    vector<ParticleNiv1 *> mPartNiv1;
    
    SensorManager * mSensorManager;
};

void HodginParticlesApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'c' )
    {
        mSensorManager->Calibrate();
    }
}

void HodginParticlesApp::prepareSettings( Settings *settings )
{
	settings->setWindowSize( 1024, 768 );
    Tools::setWindowSize(1024, 768);
	settings->setFrameRate( 60.0f );
	settings->setFullScreen( false );
}

void HodginParticlesApp::setup()
{
    Shared::setup();
    
    if(KinectManager::IsAvailable())
    {
        mSensorManager = new KinectManager();
    }
    else
    {
        // if no kinect is connected, use video.
        mSensorManager = new CaptureManager();
    }
    
    mSensorManager->setup();

    
    for(int i = 0; i < NUM_PARTICLE_NIV_1; i++)
    {
        mPartNiv1.push_back(new ParticleNiv1(i % Shared::sAnchors.size()));
    }
    
}

void HodginParticlesApp::update()
{
    mSensorManager->update();
    mSensorManager->UpdateAnchors();
    
    // update part Niv 1
    for(int i = 0; i < mPartNiv1.size(); i++)
    {
        mPartNiv1[i]->update(0.033f);
    }
    
    UpdateSubdivisions(0.033f);
}

void HodginParticlesApp::UpdateSubdivisions(float aTimeInterval)
{
    
    Subdivision * lSubdivision = Subdivision::getInstance();
    lSubdivision->Update(aTimeInterval);
    lSubdivision->Clear();
    
    for(int i = 0; i < Shared::sAnchors.size(); i++)
    {
        lSubdivision->AddAnchor(Shared::sAnchors[i]);
    }
    
    for(int i = 0; i < mPartNiv1.size(); i++)
    {
        lSubdivision->Add(mPartNiv1[i]);
    }
}

void HodginParticlesApp::draw()
{
//    mSensorManager->draw();
    gl::clear(Color(30, 220, 105));
    
    // draw part
    for(int i = 0; i < mPartNiv1.size(); i++)
    {
        mPartNiv1[i]->drawDebug(0);
    }
}

CINDER_APP_BASIC( HodginParticlesApp, RendererGl )