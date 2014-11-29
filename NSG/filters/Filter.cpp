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
#include "Program.h"
#include "ResourceMemory.h"
#include "Node.h"
#include "App.h"

namespace NSG
{
	Filter::Filter(const std::string& name, PTexture input, int output_width, int output_height, ProgramFlags flags)
        : app_(*App::this_),
		technique_(new Technique),
          pass_(new Pass),
		  pMaterial_(app_.GetOrCreateMaterial(name)),
          pMesh_(app_.CreatePlaneMesh(2, 2, 2, 2)),
          pRender2Texture_(new Render2Texture(output_width, output_height, UseBuffer::NONE)),
          name_(name),
          node_(new Node(name))
    {
        technique_->Add(pass_);
        pMaterial_->SetTexture0(input);
        pMaterial_->SetTechnique(technique_);
		program_ = app_.GetOrCreateProgram(name);
		program_->SetFlags(flags);
        pass_->SetProgram(program_);
    }

    Filter::~Filter()
    {

    }

    void Filter::SetInputTexture(PTexture input)
    {
        pMaterial_->SetTexture0(input);
    }

	PTexture Filter::GetInputTexture() const
	{
		return pMaterial_->GetTexture0();
	}

    bool Filter::Render()
    {
        if (!pMesh_->IsReady() || !pMaterial_->IsReady() || !pRender2Texture_->IsReady())
            return false;

        CHECK_GL_STATUS(__FILE__, __LINE__);

		Camera* pCurrent = Graphics::this_->GetCamera();
		Graphics::this_->SetCamera(nullptr);

        pRender2Texture_->Begin();

        Graphics::this_->SetNode(node_.get());
        Graphics::this_->Set(pMesh_.get());
        Graphics::this_->Set(pMaterial_.get());

        bool drawn = technique_->Render();

        pRender2Texture_->End();

		Graphics::this_->SetCamera(pCurrent);

        CHECK_GL_STATUS(__FILE__, __LINE__);

        return drawn;

    }

    PTexture Filter::GetTexture() const
    {
        return pRender2Texture_->GetTexture();
    }
}
