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
#include "Button.h"
#include "FontAtlas.h"
#include "Material.h"
#include "TextMesh.h"
#include "Window.h"
#include "Scene.h"
#include "Graphics.h"
#include "Check.h"
#include "Pass.h"
#include "RectangleMesh.h"

namespace NSG
{
    Button::Button(const std::string& name)
        : Control(name),
		textMaterial_(Material::Create(GetUniqueName("TextMaterial"))),
          hAlign_(CENTER_ALIGNMENT),
          vAlign_(MIDDLE_ALIGNMENT)
    {
		SetMesh(Mesh::CreateClass<RectangleMesh>("ButtonMesh"));
		SetMaterial(Material::GetOrCreate(name_));
		material_->SetAlpha(0);
        textMaterial_->SetDiffuseColor(ColorRGB(COLOR_BLACK));
		textMaterial_->SetRenderPass(RenderPass::TEXT);
    }

    Button::~Button()
    {
    }

    void Button::SetResources()
    {
        if (atlas_)
        {
            if (text_.empty())
                textNode_ = nullptr;
            else
            {
                if (!textNode_)
                {
                    textNode_ = CreateChild<SceneNode>(name_);
                    textNode_->SetInheritScale(false);
                    textNode_->DisableFlags((int)SceneNodeFlag::ALLOW_RAY_QUERY);
                    textNode_->SetMaterial(textMaterial_);
                }
                textNode_->SetMesh(atlas_->GetOrCreateMesh(text_, hAlign_, vAlign_));
            }
            textMaterial_->SetTextMap(atlas_->GetTexture());
        }
        else
            textNode_  = nullptr;
    }

    void Button::SetTextAlignment(HorizontalAlignment hAlign, VerticalAlignment vAlign)
    {
        if (hAlign_ != hAlign || vAlign_ != vAlign)
        {
            hAlign_ = hAlign;
            vAlign_ = vAlign;
            SetResources();
        }
    }

    void Button::SetText(const std::string& text)
    {
        if (text_ != text)
        {
            text_ = text;
            SetResources();
        }
    }

    void Button::SetAtlas(PFontAtlas atlas)
    {
        if (atlas_ != atlas)
        {
            atlas_ = atlas;
            SetResources();
        }
    }
}