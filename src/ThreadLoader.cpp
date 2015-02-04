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

#include "ThreadLoader.h"
#include "Settings.h"

using namespace ci;
using namespace ci::app;
using namespace std;

ThreadLoader::ThreadLoader(void)
{
	mLoadedList.clear();
    mToLoadList.clear();

    mThread = make_shared< thread >(bind(&ThreadLoader::threadLoadLoop, this));
}

ThreadLoader::~ThreadLoader(void)
{
	// gracefully abort current threads:
	deque<shared_ptr<thread>>::iterator itr;

	// wait for all threads to finish
    mThread->join();
}

void ThreadLoader::setup()
{
}

TextureList ThreadLoader::getLoaded()
{
    TextureList rList = mLoadedList;
    mLoadedList.clear();
    return rList;
}

void ThreadLoader::load(fs::path media)
{
    mToLoadMutex.lock();
    mToLoadList.push_back(media);
    mToLoadMutex.unlock();
}

Area ThreadLoader::getBounds()
{
    return Area(0,0,100,100);
}

Vec2i ThreadLoader::getSize()
{
    return Vec2i(100,100);
}

void ThreadLoader::threadLoadLoop()
{
    int lToLoadSize = 0;
    fs::path lMediaPath;
    while(1)
    {
        mToLoadMutex.lock();
        lToLoadSize = mToLoadList.size();
        mToLoadMutex.unlock();

        if(lToLoadSize > 0)
        {
            mToLoadMutex.lock();
            for(std::list< fs::path >::iterator it = mToLoadList.begin(); it != mToLoadList.end(); it++)
            {
                lMediaPath = *it;
                mToLoadList.erase(it);
                mToLoadMutex.unlock();
                threadLoad(lMediaPath);
                std::chrono::milliseconds dura( 500 );
                std::this_thread::sleep_for( dura );
                break;
            }
        }
        else
        {
            std::chrono::milliseconds dura( 100 );
            std::this_thread::sleep_for( dura );
        }
    }
}

void ThreadLoader::threadLoad(fs::path aMedia)
{
    qtime::MovieGlRef lMovie;
    
    try {
        lMovie = qtime::MovieGl::create( aMedia );

        // copy to main thread
        mSurfaceMutex.lock();
        mLoadedList.push_back(lMovie);
        mSurfaceMutex.unlock();
    }
    catch( ... ) {
        console() << "Unable to load the movie." << std::endl;
        lMovie->reset();
    }
    
    return;
}
