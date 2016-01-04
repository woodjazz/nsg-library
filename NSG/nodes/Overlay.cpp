/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2016 Néstor Silveira Gorski

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
#include "Overlay.h"
#include "Material.h"
#include "FontAtlas.h"
#include "ResourceFile.h"
#include "Texture2D.h"
#include "TextMesh.h"

namespace NSG
{
    Overlay::Overlay(const std::string& name)
        : SceneNode(name),
          hAlign_(CENTER_ALIGNMENT),
          vAlign_(MIDDLE_ALIGNMENT),
          font_(std::make_shared<FontAtlas>())
    {

    }

    Overlay::~Overlay()
    {
    }

    void Overlay::SetMaterialName(const std::string& name)
    {
        SetMaterial(Material::GetOrCreate(name));
        if(font_->IsReady())
        {
        	GetMaterial()->SetTextMap(font_->GetTexture());
        	SetMesh(font_->GetOrCreateMesh(text_, hAlign_, vAlign_));
        }
    }

    void Overlay::SetFont(const std::string& xmlName, const std::string& atlasName)
    {
        font_->Set(Resource::GetOrCreate<ResourceFile>(xmlName));
        auto atlasTexture = std::make_shared<Texture2D>(Resource::GetOrCreate<ResourceFile>(atlasName));
        if(GetMaterial())
        	GetMaterial()->SetTextMap(atlasTexture);
        font_->SetTexture(atlasTexture);
        SetMesh(font_->GetOrCreateMesh(text_, hAlign_, vAlign_));
    }

    void Overlay::SetText(const std::string& text, HorizontalAlignment hAlign, VerticalAlignment vAlign)
    {
        if (text_ != text || hAlign_ != hAlign || vAlign_ != vAlign)
        {
            text_ = text;
            hAlign_ = hAlign;
            vAlign_ = vAlign;
            if(font_)
            	SetMesh(font_->GetOrCreateMesh(text_, hAlign_, vAlign_));
        }
    }
}
