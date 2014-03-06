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

#include <cstdint>
#include <string>
#include "GLES2Includes.h"
#include "Types.h"

namespace NSG 
{
	namespace IMGUI
	{
		void AllocateResources();
		void ReleaseResources();
		void Begin();
		void End();
		enum ButtonType {Rectangle, Circle, Ellipse, RoundedRectangle};
		void SetButtonType(ButtonType type);
		void SetPosition(const Vertex3& position);
		const Vertex3& GetPosition();
		void SetSize(const Vertex3& size);
		const Vertex3& GetSize();
		void SetFont(const std::string& fontFile, int fontSize);
		void AdjustButton2Text(bool status);
		void Fill(bool enable);
		void SetNormalColor(Color color);
		void SetHotColor(Color color);
		void SetActiveColor(Color color);
		bool Button(GLushort id, const std::string& text);
		void ViewChanged(int32_t width, int32_t height);
        void OnMouseMove(float x, float y);
        void OnMouseDown(float x, float y);
        void OnMouseUp();

	}
}