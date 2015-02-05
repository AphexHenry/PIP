/*
 Copyright (C)2010 Paul Houx
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include "cinder/app/AppBasic.h"
#include "cinder/Thread.h"
#include "cinder/qtime/QuickTime.h"


typedef std::list< ci::qtime::MovieGlRef > TextureList;

class ThreadLoader
{
public:
	ThreadLoader(void);
	virtual ~ThreadLoader(void);

	void setup();

    void load(ci::fs::path aMedia);

	ci::Area	getBounds();
	ci::Vec2i	getSize();
    
    TextureList getLoaded();
    
protected:

    bool mEndThread;
    
    boost::signals2::mutex	mSurfaceMutex;
    boost::signals2::mutex	mToLoadMutex;
    std::shared_ptr< std::thread > mThread;

    std::list< ci::fs::path > mToLoadList;
    TextureList mLoadedList;

    void threadLoad(ci::fs::path url);
    void threadLoadLoop();
};
