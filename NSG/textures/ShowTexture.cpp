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
#include "Check.h"
#include "Material.h"
#include "QuadMesh.h"
#include "Camera.h"
#include "Pass.h"
#include "Graphics.h"
#include "Program.h"
#include "Scene.h"
#include "SceneNode.h"
#include "Renderer.h"
#include "Util.h"

namespace NSG
{
    ShowTexture::ShowTexture()
    {
        auto name = GetUniqueName("NSGShowTexture");
        scene_ = std::make_shared<Scene>(name);
        material_ = std::make_shared<Material>(name);
        //material->SetSerializable(false);
        mesh_ = std::make_shared<QuadMesh>(name);
        node_ = scene_->CreateChild<SceneNode>(name);
		node_->SetMesh(mesh_);
		node_->SetMaterial(material_);
        pass_ = std::make_shared<Pass>();
        pass_->EnableDepthTest(false);
 
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
        Renderer::GetPtr()->Render(pass_.get(), scene_.get(), nullptr, node_.get(), nullptr);
    }
}