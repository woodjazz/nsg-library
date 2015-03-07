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
#include "Button.h"
#include "App.h"
#include "FontAtlas.h"
#include "Material.h"
#include "TextMesh.h"
#include "Window.h"
#include "Scene.h"
#include "Graphics.h"
#include "Check.h"
#include "App.h"
#include "RectangleMesh.h"

namespace NSG
{
    Button::Button(const std::string& name)
        : SceneNode(name),
          signalButtonMouseDown_(new Signal<int>()),
          signalButtonMouseUp_(new Signal<int>()),
          pushed_(false)
    {
		textMaterial_ = App::this_->GetOrCreateMaterial(name_ + "text");
		SetMaterial(App::this_->GetOrCreateMaterial(name_));
		material_->SetSolid(false);
    }

    Button::~Button()
    {
		Invalidate();
    }

    void Button::AllocateResources()
    {
		SetMesh(App::this_->CreateRectangleMesh());
		textNode_ = CreateChild<SceneNode>(textMaterial_->GetName());
		textNode_->SetMaterial(textMaterial_);

        auto window = Graphics::this_->GetWindow();
        if(window)
            SetWindow(window);

        auto scene = GetScene();

		slotNodeMouseDown_ = scene->signalNodeMouseDown_->Connect([&](SceneNode * node, int button, float x, float y)
        {
            if(node == this || node == textNode_.get())
            {
                pushed_ = true;
                signalButtonMouseDown_->Run(button);
            }
        });
    }

    void Button::SetWindow(Window* window)
    {
        if (window)
        {
            slotMouseUp_ = window->signalMouseUp_->Connect([&](int button, float x, float y)
            {
				if (pushed_)
				{
					pushed_ = false;
					signalButtonMouseUp_->Run(button);
				}
            });
        }
        else
        {
            slotMouseUp_ = nullptr;
        }
    }


    void Button::SetText(PFontAtlas atlas, const std::string& text, HorizontalAlignment hAlign, VerticalAlignment vAlign)
    {
		CHECK_CONDITION(IsReady(), __FILE__, __LINE__);
		textMaterial_->SetTextMap(atlas->GetTexture());
        auto mesh = atlas->GetOrCreateMesh(text, hAlign, vAlign);
        textNode_->SetMesh(mesh);
    }
}