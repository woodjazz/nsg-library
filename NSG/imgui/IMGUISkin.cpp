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
#include "IMGUIStyle.h"
#include "Technique.h"
#include "Pass.h"
#include "Material.h"
#include "PlaneMesh.h"
#include "CircleMesh.h"
#include "RoundedRectangleMesh.h"
#include "RectangleMesh.h"
#include "Program.h"


namespace NSG
{
    namespace IMGUI
    {
        Skin::Skin()
            : mainWindowStyle_(new WindowStyle),
              windowStyle_(new WindowStyle),
              lineStyle_(new LineStyle),
              areaStyle_(new AreaStyle),
              labelStyle_(new LabelStyle),
              buttonStyle_(new ButtonStyle),
              checkButtonStyle_(new CheckButtonStyle),
              textStyle_(new TextStyle),
              vSliderStyle_(new SliderStyle),
              hSliderStyle_(new SliderStyle),
              vThumbSliderStyle_(new Style),
              hThumbSliderStyle_(new Style),
              stencilMaterial_(new Material("stencil")),
              stencilPass_(new Pass)
        {
            {
                mainWindowStyle_->hotMaterial_->SetColor(Color(0, 0, 0, 0));
                mainWindowStyle_->activeMaterial_->SetColor(Color(0, 0, 0, 0));
                mainWindowStyle_->normalMaterial_->SetColor(Color(0, 0, 0, 0));
            }

            PProgram program(new Program("", Program::STENCIL));
            stencilPass_->SetProgram(program);
            stencilPass_->EnableDepthTest(false);
            stencilPass_->EnableDepthBuffer(false);
            stencilPass_->EnableStencilTest(true);
            stencilPass_->EnableColorBuffer(false);
            stencilPass_->SetBlendMode(BLEND_NONE);
        }
    }
}