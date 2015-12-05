/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
#include "LevelResources.h"

std::vector<PResource> LevelResources::resources_;

LevelResources::LevelResources(PWindow window, std::vector<const char*> resourceNames)
    : Level(window),
      resourceNames_(resourceNames),
      percentage_(0),
      loaded_(false)
{
    slotDrawGUI_ = window->SigDrawIMGUI()->Connect([this]()
    {
        if (!loaded_)
        {
            const ImGuiWindowFlags hierarchyFlags = ImGuiWindowFlags_NoMove |
                                                    ImGuiWindowFlags_NoCollapse |
                                                    ImGuiWindowFlags_AlwaysAutoResize |
                                                    ImGuiWindowFlags_NoTitleBar |
                                                    ImGuiWindowFlags_NoScrollWithMouse |
                                                    ImGuiWindowFlags_NoScrollbar |
                                                    ImGuiWindowFlags_NoBringToFrontOnFocus;

            const ImGuiIO& io = ImGui::GetIO();
            auto mainWindowSize = io.DisplaySize;
            mainWindowSize.x *= 0.4f;
            mainWindowSize.y *= 0.5f;

            if (ImGui::Begin("", nullptr, hierarchyFlags))
            {
                ImGui::SetCursorPos(mainWindowSize);
                ImGui::Text("Loading: %d", percentage_);
            }
            ImGui::End();
        }
        else
        {
            slotDrawGUI_ = nullptr;
            SigNextLevel()->Run();
        }
    });

    slotUpdate_ = Engine::SigUpdate()->Connect([this](float deltaTime)
    {
        auto n = resourceNames_.size();
        while (resources_.size() != n)
        {
            auto idx = resources_.size();
            auto name = resourceNames_.at(idx);
            auto resource = Resource::GetOrCreateClass<ResourceFile>(name);
            if (!resource->IsReady())
                break;
            resources_.push_back(resource);
            percentage_ = (unsigned)(100 * idx / n);
            window_->RenderFrame();
            if (resources_.size() == n)
            {
                slotUpdate_ = nullptr;
                SigNextLevel()->Run();
            }
        }
    });
}

LevelResources::~LevelResources()
{

}
