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
#pragma once
#include "Types.h"
#include "Control.h"

namespace NSG
{
	class Button : public Control
	{
	public:
		Button(const std::string& name);
		~Button();
		void SetTextAlignment(HorizontalAlignment hAlign, VerticalAlignment vAlign);
		void SetText(const std::string& text);
		void SetAtlas(PFontAtlas atlas);
		void ScaleText(bool enable);
		PMaterial GetTextMaterial() const { return textMaterial_;}
		PSceneNode GetTextNode() const { return textNode_; }
	private:
		void SetupLayer();
		void SetResources();
        PSceneNode textNode_;
		PMaterial textMaterial_;
		std::string text_;
		HorizontalAlignment hAlign_;
		VerticalAlignment vAlign_;
		PFontAtlas atlas_;
	};
}