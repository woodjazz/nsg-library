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
#include "Pass2Texture.h"
#include "Render2Texture.h"
#include "Graphics.h"
#include "Program.h"
#include "Technique.h"
#include "Material.h"
#include "Batch.h"
#include "Mesh.h"
#include "Check.h"

namespace NSG
{
    Pass2Texture::Pass2Texture(const std::string& name, Technique* technique, int width, int height, UseBuffer buffer)
        : Pass(technique),
          render2Texture_(std::make_shared<Render2Texture>(name, width, height, buffer))
    {

    }

    Pass2Texture::~Pass2Texture()
    {

    }

    PTexture Pass2Texture::GetTexture() const
    {
        return render2Texture_->GetTexture();
    }

    void Pass2Texture::Add(PPass pass, SceneNode* node, PMesh mesh)
    {
        CHECK_ASSERT(mesh, __FILE__, __LINE__);
        passes_.push_back(PassData {pass, node, mesh});
    }

    void Pass2Texture::Draw()
    {
        if (render2Texture_->Begin())
        {
            for (auto& pass : passes_)
            {
				auto material = pass.pass_->GetProgram()->GetMaterial();
				if (material)
				{
					Batch batch(material, pass.mesh_.get());
					batch.Add(pass.node_);
					batch.Draw();
				}
				else
				{
					auto graphics = Graphics::this_;
					graphics->SetNode(pass.node_);
					graphics->SetMesh(pass.mesh_.get());
					pass.pass_->Draw();
				}
            }

            render2Texture_->End();
        }
    }
}