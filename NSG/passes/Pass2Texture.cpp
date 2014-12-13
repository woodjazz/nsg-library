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
#include "Check.h"

namespace NSG
{
    Pass2Texture::Pass2Texture(int width, int height, UseBuffer buffer)
        : render2Texture_(new Render2Texture(width, height, buffer))
    {

    }

    Pass2Texture::~Pass2Texture()
    {

    }

    PTexture Pass2Texture::GetTexture() const
    {
        return render2Texture_->GetTexture();
    }

    void Pass2Texture::Add(PPass pass, SceneNode* node, PMaterial material, PMesh mesh)
    {
		CHECK_ASSERT(mesh, __FILE__, __LINE__);
        passes_.push_back(PassData {pass, node, material, mesh});
    }

    void Pass2Texture::Render()
    {
        if (render2Texture_->Begin())
        {
            for(auto& pass: passes_)
            {
                Graphics::this_->Set(pass.mesh_.get());
                Graphics::this_->Set(pass.material_.get());
				Graphics::this_->SetNode(pass.node_);
                pass.pass_->Render();
            }

            render2Texture_->End();
        }
    }
}