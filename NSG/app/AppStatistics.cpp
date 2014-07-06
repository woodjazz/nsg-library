/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://nsg-library.googlecode.com/

Copyright (c) 2014-2015 Néstor Silveira Gorski

-------------------------------------------------------------------------------
This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "AppStatistics.h"
#include "AppConfiguration.h"
#include "IMGUIContext.h"
#include "IMGUISkin.h"


namespace NSG 
{
	AppStatistics::AppStatistics()
    : collect_(true),
	vertexBuffers_(0),
	indexBuffers_(0)
	{
		memset(&counters_[0], 0, sizeof(counters_));
	}

	AppStatistics::~AppStatistics()
	{

	}

	bool AppStatistics::Collect(bool collect)
	{
		bool status = collect_;
		collect_ = collect;
		return status;
	}

	void AppStatistics::Reset()
	{
		tpReset_ = Clock::now();
	}

	void AppStatistics::Add2LastSecond(Counter& counter, size_t n)
	{
		if(!collect_)
			return;

		Milliseconds ms = std::chrono::duration_cast<Milliseconds>(Clock::now() - tpReset_);

		if(ms.count() >= 1000)
		{
            for(int i=0; i<MAX_COUNTERS; i++)
            {
                Counter& obj = counters_[i];
                obj.first = obj.second;
                obj.second = 0;
            }
			Reset();
		}		
        else
        {
            counter.second += n;
        }
	}

	void AppStatistics::ResetVertexBuffer()
	{
		vertexBuffers_ = 0;
	}

	void AppStatistics::ResetIndexBuffer()
	{
		indexBuffers_ = 0;
	}

	void AppStatistics::NewVertexBuffer()
	{
		++vertexBuffers_;
	}

	void AppStatistics::NewIndexBuffer()
	{
		++indexBuffers_;
	}

	void AppStatistics::NewFrame()
	{
		Add2LastSecond(counters_[FRAMES], 1);
	}

	void AppStatistics::NewDrawCall()
	{
		Add2LastSecond(counters_[DRAWCALLS], 1);
	}

	void AppStatistics::NewTriangles(size_t n)
	{
		Add2LastSecond(counters_[TRIANGLES], n);	
	}

	void AppStatistics::RenderGUIWindow()
	{
        int fontSize = IMGUI::Context::this_->pSkin_->fontSize_;
        IMGUI::Context::this_->pSkin_->fontSize_ = 14;
		Collect(false);
		static std::string emptys;

		IMGUISpacer(0, 10);

		const int MAX_LABEL_SIZE = 20;
        size_t fps = counters_[FRAMES].first;;
	    std::stringstream ss;
	    ss << "FPS:" << fps;
		IMGUILabel(MAX_LABEL_SIZE, ss.str(), 0, 15);

	    ss.str(emptys);

        size_t drawCalls = counters_[DRAWCALLS].first;
        if(fps) drawCalls/=fps;
	    ss << "DrawCalls:" << drawCalls;
		IMGUILabel(MAX_LABEL_SIZE, ss.str(), 0, 15);

	    ss.str(emptys);

        size_t triangles = counters_[TRIANGLES].first;
        if(fps) triangles/=fps;
	    ss << "Triangles:" << triangles;
		IMGUILabel(MAX_LABEL_SIZE, ss.str(), 0, 15);

	    ss.str(emptys);

		ss << "VertexBuffers:" << vertexBuffers_;
		IMGUILabel(MAX_LABEL_SIZE, ss.str(), 0, 15);

	    ss.str(emptys);

	    ss << "IndexBuffers:" << indexBuffers_;
		IMGUILabel(MAX_LABEL_SIZE, ss.str(), 0, 15);

	    Collect(true);

        IMGUI::Context::this_->pSkin_->fontSize_ = fontSize;
	}
}