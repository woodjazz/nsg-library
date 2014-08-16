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
#include "IMGUIObject.h"
#include "Types.h"
#include <string>
namespace NSG
{
	namespace IMGUI
	{
		class Title : public Object
		{
		public:
			Title(const std::string& text, TitleStyle& style);
			~Title();
			Vertex3 Render();
			virtual void UpdateControl() override;
		private:
			const std::string& currentText_;
			TitleStyle& titleStyle_;
			PTextMesh pTextMesh_;
			Vertex3 windowOffset_;
		};
	}
}