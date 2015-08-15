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

namespace NSG
{
    ShowTexture::ShowTexture()
        : material_(std::make_shared<Material>(GetUniqueName("NSGShowTexture"))),
          mesh_(std::make_shared<QuadMesh>(GetUniqueName("NSGShowTexture"))),
          node_(std::make_shared<SceneNode>("NSGShowTexture")),
          graphics_(Graphics::GetPtr())
    {
        //auto pass = material_->GetTechnique()->GetPass(0);
        material_->SetSerializable(false);
        //pass->EnableDepthTest(false);
    }

    ShowTexture::~ShowTexture()
    {
    }

    void ShowTexture::SetColortexture(PTexture texture)
    {
		material_->SetTexture(texture);
		material_->SetRenderPass(RenderPass::SHOW_TEXTURE0);
		material_->FlipYTextureCoords(true);
    }

    void ShowTexture::SetNormal(PTexture texture)
    {
		material_->SetTexture(texture);
		material_->SetRenderPass(RenderPass::SHOW_TEXTURE0);
    }

    void ShowTexture::SetFont(PTexture texture)
    {
		material_->SetTexture(texture);
		material_->SetRenderPass(RenderPass::TEXT);
    }

    void ShowTexture::Show()
    {
        if (material_->IsReady() && mesh_->IsReady())
        {
            CHECK_GL_STATUS(__FILE__, __LINE__);

            Camera* pCurrent = graphics_->GetCamera();
            graphics_->SetCamera(nullptr);
			Pass pass;
			pass.EnableDepthTest(false);
            graphics_->SetMesh(mesh_.get());
			if (graphics_->SetupProgram(&pass, node_.get(), material_.get(), nullptr))
				graphics_->DrawActiveMesh();

            graphics_->SetCamera(pCurrent);

            CHECK_GL_STATUS(__FILE__, __LINE__);
        }

    }
}