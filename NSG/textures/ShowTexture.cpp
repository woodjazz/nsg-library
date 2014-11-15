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
#include "ShowTexture.h"
#include "Types.h"
#include "Material.h"
#include "PlaneMesh.h"
#include "Context.h"
#include "Camera.h"
#include "Pass.h"
#include "Graphics.h"
#include "Program.h"
#include "Node.h"
#include "App.h"
#include "Util.h"

namespace NSG
{
    ShowTexture::ShowTexture()
        : app_(*App::this_),
          pass_(new Pass),
		  material_(app_.CreateMaterial(GetUniqueName("NSGShowTexture"))),
          program_(app_.GetOrCreateProgram("NSGShowTexture")),
          mesh_(app_.CreatePlaneMesh(2, 2, 2, 2)),
          node_(new Node("ShowTexture"))
    {
        pass_->SetProgram(program_);
        material_->SetSerializable(false);
        pass_->EnableDepthTest(false);
    }

    ShowTexture::~ShowTexture()
    {
        pass_ = nullptr;
        Context::RemoveObject(this);
    }

    bool ShowTexture::IsValid()
    {
        return material_->IsReady() && mesh_->IsReady();
    }

    void ShowTexture::AllocateResources()
    {

    }

    void ShowTexture::ReleaseResources()
    {

    }

    void ShowTexture::SetNormal(PTexture texture)
    {
        program_->SetFlags((int)ProgramFlag::SHOW_TEXTURE0);
        material_->SetTexture0(texture);
    }

    void ShowTexture::SetFont(PTexture texture)
    {
        program_->SetFlags((int)ProgramFlag::TEXT);
        material_->SetTexture0(texture);
    }

    void ShowTexture::Show()
    {
        if (IsReady())
        {
            CHECK_GL_STATUS(__FILE__, __LINE__);

            Camera* pCurrent = Camera::Deactivate();

            Graphics::this_->Set(material_.get());
            Graphics::this_->SetNode(node_.get());
            Graphics::this_->Set(mesh_.get());
            pass_->Render();

            Camera::Activate(pCurrent);

            CHECK_GL_STATUS(__FILE__, __LINE__);
        }

    }
}