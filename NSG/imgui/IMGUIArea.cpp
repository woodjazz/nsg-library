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
#include "IMGUIArea.h"
#include "IMGUIContext.h"
#include "IMGUIState.h"
#include "IMGUISkin.h"
#include "IMGUILayoutManager.h"
#include "TextMesh.h"
#include "SceneNode.h"
#include "Technique.h"
#include "Pass.h"
#include "Material.h"
#include "Keys.h"

namespace NSG
{
	namespace IMGUI
	{
		Area::Area(GLushort id, LayoutType type, int percentageX, int percentageY)
		: Object(id, type, percentageX, percentageY)
		{
            CHECK_ASSERT(type == LayoutType::Horizontal || type == LayoutType::Vertical, __FILE__, __LINE__);
            area_->isReadOnly_ = IsReadOnly();
		}

		Area::~Area()
		{

		}

		void Area::operator()()
		{
//            Context::this_->pSkin_->stencilPass_->GetStencilMask()->SetStencilFunc(GL_GEQUAL, area_->stencilRefValue_, area_->stencilMaskValue_);

			Update();
		}	

		PTechnique Area::GetActiveTechnique() const
		{
			return Context::this_->pSkin_->areaTechnique_;
		}

		PTechnique Area::GetHotTechnique() const
		{
			return Context::this_->pSkin_->areaTechnique_;
		}

		PTechnique Area::GetNormalTechnique() const
		{
			return Context::this_->pSkin_->areaTechnique_;
		}

	}
}