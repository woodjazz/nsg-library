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
#include "IMGUIStyle.h"
#include "Technique.h"
#include "Pass.h"
#include "Material.h"
#include "PlaneMesh.h"
#include "CircleMesh.h"
#include "RoundedRectangleMesh.h"
#include "RectangleMesh.h"
#include "ProgramSimpleColor.h"
#include "ProgramUnlit.h"
#include "ProgramWhiteColor.h"

namespace NSG
{
    namespace IMGUI
    {
        Style::Style()
            : fontSize_(18),
              textMaxLength_(std::numeric_limits<int>::max()),
              activeTechnique_(new Technique),
              normalTechnique_(new Technique),
              hotTechnique_(new Technique)
        {
            PProgram unlitProgram(new ProgramUnlit);

            PMaterial pActiveMaterial(new Material);
            pActiveMaterial->SetProgram(unlitProgram);
            pActiveMaterial->SetColor(Color(1, 0, 0, 0.7f));

            PMaterial pNormalMaterial(new Material);
            pNormalMaterial->SetProgram(unlitProgram);
            pNormalMaterial->SetColor(Color(0, 1, 0, 0.7f));

            PMaterial pHotMaterial(new Material);
            pHotMaterial->SetProgram(unlitProgram);
            pHotMaterial->SetColor(Color(0, 0, 1, 0.7f));

            PMesh areaMesh(new PlaneMesh(2, 2, 2, 2, GL_STATIC_DRAW));
            PMesh controlMesh = areaMesh;

            PPass activePass(new Pass);
            activePass->Set(pActiveMaterial);
            activePass->Add(nullptr, controlMesh);
            activePass->EnableDepthTest(false);
            activePass->EnableStencilTest(true);

            PPass normalPass(new Pass);
            normalPass->Set(pNormalMaterial);
            normalPass->Add(nullptr, controlMesh);
            normalPass->EnableDepthTest(false);
            normalPass->EnableStencilTest(true);

            PPass hotPass(new Pass);
            hotPass->Set(pHotMaterial);
            hotPass->Add(nullptr, controlMesh);
            hotPass->EnableDepthTest(false);
            hotPass->EnableStencilTest(true);

            activeTechnique_->Add(activePass);
            normalTechnique_->Add(normalPass);
            hotTechnique_->Add(hotPass);
        }

        Style::Style(const Style& obj)
            : fontSize_(obj.fontSize_),
              textMaxLength_(obj.textMaxLength_),
              activeTechnique_(obj.activeTechnique_),
              normalTechnique_(obj.normalTechnique_),
              hotTechnique_(obj.hotTechnique_)
        {

        }
    }
}