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
#include "Filter.h"
#include "QuadMesh.h"
#include "Material.h"
#include "Camera.h"
#include "Check.h"
#include "Pass.h"
#include "Graphics.h"
#include "Program.h"
#include "FrameBuffer.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "Node.h"

namespace NSG
{
    Filter::Filter(const std::string& name)
        : pMaterial_(Material::GetOrCreate(name)),
          pMesh_(Mesh::GetOrCreateClass<QuadMesh>("FilterMesh")),
          frameBuffer_(std::make_shared<FrameBuffer>(name, FrameBuffer::Flag::COLOR | FrameBuffer::Flag::COLOR_USE_TEXTURE)),
          name_(name),
          node_(std::make_shared<SceneNode>(name))
    {
        pMaterial_->SetRenderPass(RenderPass::UNLIT);
        node_->SetMaterial(pMaterial_);
        node_->SetMesh(pMesh_);
        auto graphics = Graphics::GetPtr();
        if (graphics)
        {
            auto window = graphics->GetWindow();
            if (window)
                SetWindow(window);
        }

        camera_ = std::make_shared<Camera>(name);
        camera_->EnableOrtho();
        camera_->SetNearClip(-1000);
        camera_->SetFarClip(1000);
        camera_->UnRegisterWindow();

    }

    Filter::~Filter()
    {
    }

    void Filter::SetWindow(Window* window)
    {
        frameBuffer_->SetWindow(window);
    }

    void Filter::SetInputTexture(PTexture input)
    {
		if (input_ != input)
		{
			input_ = input;
			if(input)
            {
                CHECK_ASSERT(input->GetMapType() == TextureType::COL);
				pMaterial_->SetTexture(input);
            }
		}
    }

    PTexture Filter::GetInputTexture() const
    {
        return pMaterial_->GetTexture(MaterialTexture::DIFFUSE_MAP);
    }

    void Filter::Draw()
    {
        if (input_ == nullptr || !input_->IsReady() || !frameBuffer_->IsReady() || !pMesh_->IsReady() || !pMaterial_->IsReady())
            return;
        Pass pass;
        pass.EnableDepthTest(false);
        auto graphics = Graphics::GetPtr();
        auto oldFrameBuffer = graphics->SetFrameBuffer(frameBuffer_.get());
        Renderer::GetPtr()->Render(&pass, nullptr, camera_.get(), node_.get(), nullptr);
        graphics->SetFrameBuffer(oldFrameBuffer);
    }

    PTexture Filter::GetTexture() const
    {
        return frameBuffer_->GetColorTexture();
    }
}
