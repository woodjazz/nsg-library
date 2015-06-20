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
		ShadowCamera(const Light* light);
		~ShadowCamera();
		void SetupSpot(const Camera* camera);
		void SetupPoint(const Camera* camera);
		void SetupDirectional(int split, const Camera* camera, float nearSplit, float farSplit);
		void SetCurrentCubeShadowMapFace(TextureTarget target);
		bool GetVisiblesShadowCasters(std::vector<SceneNode*>& result) const;
		float GetFarSplit() const { return farSplit_; }
		float GetInvRange() const;
		const Vector3& GetLightGlobalPosition() const;
	private:
		BoundingBox GetViewBoxAndAdjustPosition(const Frustum* frustum, bool receivers, bool casters, bool& isEmpty);
		void QuantizeDirLightShadowCamera(int split, const BoundingBox& viewBox);

		const Light* light_;
		Node dirPositiveX_;
        Node dirNegativeX_;
        Node dirPositiveY_;
        Node dirNegativeY_;
        Node dirPositiveZ_;
        Node dirNegativeZ_;
        float nearSplit_;
        float farSplit_;
        float viewRange_;
	};
}
