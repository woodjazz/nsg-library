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

namespace NSG
{
	namespace IMGUI
	{
		class Object
		{
		public:
			Object(GLushort id, bool isReadOnly, LayoutType type, int percentageX, int percentageY);
			~Object();
			bool IsStable() const;
			bool Hit(float x, float y);
			bool Hit(GLushort id, float x, float y, const PTechnique& technique);
			void Draw();
			bool Update();
			virtual void UpdateControl() {};
			bool HasFocus() const;
			bool IsActive() const;
			virtual bool NeedsKeyboard() const { return false; }
			virtual void OnFocus() {};
			virtual void OnActive() {};
			virtual void OnKey(int key) {}
			virtual void OnChar(unsigned int character) {}
			virtual PTechnique GetActiveTechnique() const;
			virtual PTechnique GetHotTechnique() const;
			virtual PTechnique GetNormalTechnique() const;
			PLayoutArea GetArea() const { return area_; }
            void FixCurrentTecnique();
            void SetStencilTestForHit();
		protected:
			GLushort id_;
			State& uistate_;
			PLayoutArea area_;
			PNode node_;
			Vertex4 areaSize_;
			PTechnique currentTechnique_;

			const float& mouseDownX_;
			const float& mouseDownY_;
			const float& mousex_;
			const float& mousey_;
			const bool& mousedown_;


		};
	}
}