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
#include "IMGUIContext.h"
#include "IMGUISkin.h"
#include "Camera.h"
#include "Node.h"
#include "Keys.h"
#include "Graphics.h"
#include "Material.h"
#include "Technique.h"
#include "Pass.h"
#include "RectangleMesh.h"
#include "IMGUILayoutManager.h"
#include "FrameColorSelection.h"
#include "IMGUIState.h"
#include "App.h"
#include "AppConfiguration.h"
#include "AppStatistics.h"
#include "IMGUI.h"
#include "Pass.h"
#include "Program.h"

namespace NSG
{
    template<> IMGUI::Context* Singleton<IMGUI::Context>::this_ = nullptr;

    namespace IMGUI
    {
        Context::Context()
            : unlitProgram_(new Program),
              controlMesh_(App::this_->CreateRectangleMesh(2, 2)),
              state_(new State),
              pSkin_(new Skin),
              pCurrentNode_(new Node("IMGUI::Context::pCurrentNode_")),
              pRootNode_(new Node("IMGUI::Context::pRootNode_")),
              pLayoutManager_(new LayoutManager(pRootNode_)),
              transparentAreaStyle_(new AreaStyle),
              viewport_(0),
              hasAppGUI_(true)
        {
            App::Add(this);
            transparentAreaStyle_->hotMaterial_->SetColor(Color(0, 0, 0, 0));
            transparentAreaStyle_->activeMaterial_->SetColor(Color(0, 0, 0, 0));
            transparentAreaStyle_->normalMaterial_->SetColor(Color(0, 0, 0, 0));
            controlMesh_->SetSerializable(false);
        }

        Context::~Context()
        {
            App::Remove(this);
            Context::this_ = nullptr;
        }

        bool Context::IsReady() const
        {
            return pLayoutManager_->IsReady();
        }

        void Context::RenderGUI()
        {
            if (hasAppGUI_)
            {
                Camera* camera = Camera::Deactivate();

                Graphics::this_->SetViewport(viewport_);

                state_->Begin();

                pCurrentNode_ = pRootNode_;

                pLayoutManager_->Render();

                state_->End();

                if (AppConfiguration::this_->showStatistics_)
                    AppStatistics::this_->Show();

                Camera::Activate(camera);

                hasAppGUI_ = pLayoutManager_->HasAppGUI();
            }
            else
            {
                Camera* camera = Camera::Deactivate();

                Graphics::this_->SetViewport(viewport_);

                if (AppConfiguration::this_->showStatistics_)
                    AppStatistics::this_->Show();

                Camera::Activate(camera);
            }
        }

        IdType Context::GetValidId()
        {
            return pLayoutManager_->GetValidId();
        }

        void Context::OnViewChanged(int width, int height)
        {
            viewport_.z = width;
            viewport_.w = height;

        }
    }

}