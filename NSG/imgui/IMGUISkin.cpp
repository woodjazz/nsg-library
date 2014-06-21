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
#include "IMGUISkin.h"
#include "Technique.h"
#include "Pass.h"
#include "Material.h"
#include "PlaneMesh.h"
#include "CircleMesh.h"
#include "RoundedRectangleMesh.h"
#include "RectangleMesh.h"
#include "ProgramSimpleColor.h"
#include "ProgramUnlit.h"

namespace NSG 
{
	namespace IMGUI
	{
		Skin::Skin() 
		: fontSize_(18),
		textMaxLength_(std::numeric_limits<int>::max()),
		windowTechnique_(new Technique),
		areaTechnique_(new Technique),
        activeTechnique_(new Technique),
        normalTechnique_(new Technique),
        hotTechnique_(new Technique),
        labelTechnique_(new Technique),
        sliderTechnique_(new Technique),
        titleTechnique_(new Technique),
        borderTechnique_(new Technique)
		{
			PProgram unlitProgram(new ProgramUnlit);

			PMaterial pActiveMaterial(new Material);
			pActiveMaterial->EnableDepthTest(false);
            pActiveMaterial->SetProgram(unlitProgram);
			pActiveMaterial->SetColor(Color(1,0,0,0.7f));
			pActiveMaterial->EnableStencilTest(true);

			PMaterial pNormalMaterial(new Material);
			pNormalMaterial->EnableDepthTest(false);
            pNormalMaterial->SetProgram(unlitProgram);
			pNormalMaterial->SetColor(Color(0,1,0,0.7f));
			pNormalMaterial->EnableStencilTest(true);

			PMaterial pHotMaterial(new Material);
			pHotMaterial->EnableDepthTest(false);
			pHotMaterial->SetProgram(unlitProgram);
			pHotMaterial->SetColor(Color(0,0,1,0.7f));
			pHotMaterial->EnableStencilTest(true);

			PMaterial pBorderMaterial(new Material);
			PProgram pBorderProgram(new ProgramUnlit);
			pBorderMaterial->EnableDepthTest(false);
			pBorderMaterial->SetProgram(pBorderProgram);
			pBorderMaterial->SetColor(Color(1,1,1,1));
			pBorderMaterial->EnableStencilTest(true);

			PMaterial labelMaterial(new Material);
			labelMaterial->EnableDepthTest(false);
            labelMaterial->SetProgram(unlitProgram);
			labelMaterial->SetColor(Color(0,0,0,0.0f));
			labelMaterial->EnableStencilTest(true);

			PMaterial windowMaterial(new Material);
            //windowMaterial->SetColor(Color(0,1,1,0.5f));
            windowMaterial->SetColor(Color(0,0,0,0));
            windowMaterial->SetProgram(unlitProgram);
            //windowMaterial->SetProgram(pProgram);
            windowMaterial->EnableDepthTest(false);
			windowMaterial->EnableStencilTest(true);
			//windowMaterial->EnableColorBuffer(false);


			PMaterial areaMaterial(new Material);
            //areaMaterial->SetColor(Color(0,1,1,0.5f));
            areaMaterial->SetColor(Color(0,0,0,0));
            areaMaterial->SetProgram(unlitProgram);
            //areaMaterial->SetProgram(pProgram);
            areaMaterial->EnableDepthTest(false);
			areaMaterial->EnableStencilTest(true);
			//areaMaterial->EnableColorBuffer(false);

			PMaterial sliderMaterial(new Material);
			sliderMaterial->EnableDepthTest(false);
            sliderMaterial->SetProgram(unlitProgram);
			sliderMaterial->SetColor(Color(0.5f,0.5f,0.5f,0.6f));
			sliderMaterial->EnableStencilTest(true);

			PMaterial titleMaterial(new Material);
			titleMaterial->EnableDepthTest(false);
            titleMaterial->SetProgram(unlitProgram);
			titleMaterial->SetColor(Color(0.5f,0.5f,1.0f,0.6f));
			titleMaterial->EnableStencilTest(true);
			

			PMesh borderMesh(new RectangleMesh(2, 2, GL_STATIC_DRAW));
            PMesh areaMesh(new PlaneMesh(2, 2, 2, 2, GL_STATIC_DRAW));
            //PMesh areaMesh(new CircleMesh(1, 32, GL_STATIC_DRAW));
            
            //PMesh controlMesh(new RoundedRectangleMesh(0.5f, 2, 2, 64, GL_STATIC_DRAW));

            PMesh controlMesh = areaMesh;

			PPass activePass(new Pass);
			activePass->Set(pActiveMaterial);
			activePass->Add(nullptr, controlMesh);

			PPass normalPass(new Pass);
			normalPass->Set(pNormalMaterial);
			normalPass->Add(nullptr, controlMesh);

			PPass hotPass(new Pass);
			hotPass->Set(pHotMaterial);
			hotPass->Add(nullptr, controlMesh);

			PPass borderPass(new Pass);
			borderPass->Set(pBorderMaterial);
			borderPass->Add(nullptr, borderMesh);
			borderPass->SetDrawMode(Pass::WIREFRAME);

            PPass windowPass(new Pass);
            //windowPass->Set(pNormalMaterial);
            windowPass->Set(windowMaterial);
			windowPass->Add(nullptr, areaMesh);


            PPass areaPass(new Pass);
            //areaPass->Set(pNormalMaterial);
            areaPass->Set(areaMaterial);
			areaPass->Add(nullptr, areaMesh);

			PPass sliderPass(new Pass);
			sliderPass->Set(sliderMaterial);
			sliderPass->Add(nullptr, areaMesh);

			PPass titlePass(new Pass);
			titlePass->Set(titleMaterial);
			titlePass->Add(nullptr, areaMesh);


			sliderTechnique_->Add(sliderPass);

			titleTechnique_->Add(titlePass);
			titleTechnique_->Add(borderPass);

			borderTechnique_->Add(borderPass);
			
			windowTechnique_->Add(windowPass);

			areaTechnique_->Add(areaPass);

			//activeTechnique_->Add(borderPass); //needed to have accurate precision in the stencil buffer
			activeTechnique_->Add(activePass);
			//activeTechnique_->Add(borderPass);

			//normalTechnique_->Add(borderPass); //needed to have accurate precision in the stencil buffer
			normalTechnique_->Add(normalPass);
			//normalTechnique_->Add(borderPass);

			//hotTechnique_->Add(borderPass); //needed to have accurate precision in the stencil buffer
			hotTechnique_->Add(hotPass);
			//hotTechnique_->Add(borderPass);

			PPass labelPass(new Pass);
			labelPass->Set(labelMaterial);
			labelPass->Add(nullptr, controlMesh);

			labelTechnique_->Add(labelPass);
		}

		Skin::Skin(const Skin& obj)
		: fontFile_(obj.fontFile_),
		fontSize_(obj.fontSize_),
		textMaxLength_(obj.textMaxLength_),
        areaTechnique_(obj.areaTechnique_),
		activeTechnique_(obj.activeTechnique_),
		normalTechnique_(obj.normalTechnique_),
		hotTechnique_(obj.hotTechnique_),
		labelTechnique_(obj.labelTechnique_)
		{
		}
	}
}