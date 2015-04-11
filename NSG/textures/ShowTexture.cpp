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
#include "ShowTexture.h"
#include "Types.h"
#include "Material.h"
#include "QuadMesh.h"
#include "Camera.h"
#include "Pass.h"
#include "Graphics.h"
#include "Program.h"
#include "Node.h"
#include "Util.h"
#include "Technique.h"

namespace NSG
{
    ShowTexture::ShowTexture()
        : material_(std::make_shared<Material>(GetUniqueName("NSGShowTexture"))),
          mesh_(std::make_shared<QuadMesh>(GetUniqueName("NSGShowTexture"))),
          node_(std::make_shared<SceneNode>("NSGShowTexture"))
    {
        auto pass = material_->GetTechnique()->GetPass(0);
        program_ = pass->GetProgram();
        material_->SetSerializable(false);
        pass->EnableDepthTest(false);
    }

    ShowTexture::~ShowTexture()
    {
    }

    void ShowTexture::SetColortexture(PTexture texture)
    {
        program_->SetFlags((int)ProgramFlag::SHOW_TEXTURE0 | (int)ProgramFlag::FLIP_Y);
        material_->SetTexture(0, texture);
    }

    void ShowTexture::SetNormal(PTexture texture)
    {
        program_->SetFlags((int)ProgramFlag::SHOW_TEXTURE0);
        material_->SetTexture(0, texture);
    }

    void ShowTexture::SetFont(PTexture texture)
    {
        program_->SetFlags((int)ProgramFlag::TEXT);
        material_->SetTexture(0, texture);
    }

    void ShowTexture::Show()
    {
        if (material_->IsReady() && mesh_->IsReady())
        {
            CHECK_GL_STATUS(__FILE__, __LINE__);

            Camera* pCurrent = Graphics::this_->GetCamera();
            Graphics::this_->SetCamera(nullptr);

            Graphics::this_->SetNode(node_.get());
            Graphics::this_->SetMesh(mesh_.get());
            material_->GetTechnique()->Draw();

            Graphics::this_->SetCamera(pCurrent);

            CHECK_GL_STATUS(__FILE__, __LINE__);
        }

    }
}