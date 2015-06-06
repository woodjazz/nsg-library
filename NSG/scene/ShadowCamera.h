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
#pragma once
#include "Types.h"
#include "Camera.h"

namespace NSG
{
	class ShadowCamera : public Camera
	{
	public:
		ShadowCamera(const std::string& name);
		~ShadowCamera();
		void Setup(const Light* light, const Window* window, const Camera* camera);
		void SetCurrentCubeShadowMapFace(TextureTarget target);
		bool IsVisible(const SceneNode* node) const;
		bool IsVisibleFromCurrentFace(const SceneNode* node) const;
		bool GetVisibles(const std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& result) const;
		bool GetVisiblesFromCurrentFace(const std::vector<SceneNode*>& nodes, std::vector<SceneNode*>& result) const;
	private:
        Node dirPositiveX_;
        Node dirNegativeX_;
        Node dirPositiveY_;
        Node dirNegativeY_;
        Node dirPositiveZ_;
        Node dirNegativeZ_;
        LightType type_;
	};
}
