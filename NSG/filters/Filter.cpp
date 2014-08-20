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
#include "Filter.h"
#include "PlaneMesh.h"
#include "Material.h"
#include "Camera.h"
#include "Check.h"
#include "Technique.h"
#include "Pass.h"
#include "Render2Texture.h"
#include "Graphics.h"

static const char* vShader = STRINGIFY(
                                 void main()
{
    gl_Position = a_position;
    v_texcoord = a_texcoord;
}
                             );

namespace NSG
{
    Filter::Filter(PTexture input, PTexture output, const char* fragment)
        : technique_(new Technique),
          pass_(new Pass),
          pMaterial_(new Material),
          pMesh_(new PlaneMesh(2, 2, 2, 2)),
          pRender2Texture_(new Render2Texture(output, true, false))
    {
        technique_->Add(pass_);
        pMaterial_->SetTexture0(input);
        pMaterial_->SetTechnique(technique_);
        pass_->SetProgram(PProgram(new Program(vShader, fragment)));
    }

    Filter::~Filter()
    {

    }

    bool Filter::Render()
    {
        if (!pMesh_->IsReady() || !pMaterial_->IsReady() || !pRender2Texture_->IsReady())
            return false;

        CHECK_GL_STATUS(__FILE__, __LINE__);

        Camera* pCurrent = Camera::Deactivate();

        pRender2Texture_->Begin();

        Graphics::this_->Set((Node*)nullptr);
        Graphics::this_->Set(pMesh_.get());
        Graphics::this_->Set(pMaterial_.get());

        bool drawn = technique_->Render();

        pRender2Texture_->End();

        Camera::Activate(pCurrent);

        CHECK_GL_STATUS(__FILE__, __LINE__);

        return drawn;

    }

    PTexture Filter::GetTexture() const
    {
        return pRender2Texture_->GetTexture();
    }
}
