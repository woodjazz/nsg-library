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
#include "IMGUIStyle.h"
#include "App.h"


namespace NSG
{
    template <> AppStatistics* Singleton<AppStatistics>::this_ = nullptr;

    AppStatistics::AppStatistics()
        : collect_(true),
          staticVertexBuffers_(0),
          staticIndexBuffers_(0),
          dynamicVertexBuffers_(0),
          dynamicIndexBuffers_(0),
          triangles_(0),
          drawCalls_(0),
          fps_(0),
          frames_(0)
    {
        IWindow::hasTitle_ = true;
        IWindow::resizable_ = false;
        IWindow::title_ = "Stats";
        startTime_ = Clock::now();
    }

    AppStatistics::~AppStatistics()
    {

    }

    void AppStatistics::Reset()
    {
    	Milliseconds ms = std::chrono::duration_cast<Milliseconds>(Clock::now() - startTime_);
        if(ms.count() >= 1000)
        {
            startTime_ = Clock::now();
            fps_ = frames_;
            frames_ = 0;
        }
        else
        {
        	++frames_;
        }
        
        triangles_ = 0;
        drawCalls_ = 0;
        
    }


    void AppStatistics::AddVertexBuffer(bool dynamic)
    {
        if (collect_)
        {
            if (dynamic)
                ++dynamicVertexBuffers_;
            else
                ++staticVertexBuffers_;
        }
    }

    void AppStatistics::AddIndexBuffer(bool dynamic)
    {
        if (collect_)
        {
            if (dynamic)
                ++dynamicIndexBuffers_;
            else
                ++staticIndexBuffers_;
        }
    }

    void AppStatistics::RemoveVertexBuffer(bool dynamic)
    {
        if (collect_)
        {
            if (dynamic)
                --dynamicVertexBuffers_;
            else
                --staticVertexBuffers_;
        }
    }

    void AppStatistics::RemoveIndexBuffer(bool dynamic)
    {
        if (collect_)
        {
            if (dynamic)
                --dynamicIndexBuffers_;
            else
                --staticIndexBuffers_;
        }
    }

    void AppStatistics::RenderGUIWindow()
    {
        collect_ = false;

        static std::string emptys;

        IMGUISpacer(100, 10);

        const float LABEL_HEIGTH = 15;
        std::stringstream ss;
        ss << "FPS:" << fps_;
        IMGUILine();
        IMGUILabel(ss.str(), 100, LABEL_HEIGTH);

        ss.str(emptys);

        ss << "DrawCalls:" << drawCalls_;
        IMGUILine();
        IMGUILabel(ss.str(), 100, LABEL_HEIGTH);

        ss.str(emptys);

        ss << "Triangles:" << triangles_;
        IMGUILine();
        IMGUILabel(ss.str(), 100, LABEL_HEIGTH);

        ss.str(emptys);

        ss << "StaticVertexBuffers:" << staticVertexBuffers_;
        IMGUILine();
        IMGUILabel(ss.str(), 100, LABEL_HEIGTH);

        ss.str(emptys);

        ss << "StaticIndexBuffers:" << staticIndexBuffers_;
        IMGUILine();
        IMGUILabel(ss.str(), 100, LABEL_HEIGTH);

        ss.str(emptys);

        ss << "DynamicVertexBuffers:" << dynamicVertexBuffers_;
        IMGUILine();
        IMGUILabel(ss.str(), 100, LABEL_HEIGTH);

        ss.str(emptys);

        ss << "DynamicIndexBuffers:" << dynamicIndexBuffers_;
        IMGUILine();
        IMGUILabel(ss.str(), 100, LABEL_HEIGTH);

        collect_ = true;


    }
}
