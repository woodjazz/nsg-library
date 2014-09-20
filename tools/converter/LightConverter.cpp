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

#include "LightConverter.h"
#include "TextureFile.h"
#include "Check.h"
#include "Util.h"
#include "Constants.h"
#include "Pass.h"
#include "Technique.h"
#include "assimp/light.h"

namespace NSG
{
	LightConverter::LightConverter(const aiLight* light, Scene* scene) 
		: Light(light->mName.C_Str(), scene)
	{
		switch (light->mType)
		{
		case aiLightSource_DIRECTIONAL:
			SetDirectional();
			break;
		case aiLightSource_POINT:
			SetPoint();
			break;
		case aiLightSource_SPOT:
		{
			float spotCutOff = light->mAngleOuterCone;
			SetSpotLight(spotCutOff);
			break;
		}
		default:
			CHECK_ASSERT(false, __FILE__, __LINE__);
		}

		SetAmbientColor(Color(light->mColorAmbient.r, light->mColorAmbient.g, light->mColorAmbient.b, 1));
		SetDiffuseColor(Color(light->mColorDiffuse.r, light->mColorDiffuse.g, light->mColorDiffuse.b, 1));
		SetSpecularColor(Color(light->mColorSpecular.r, light->mColorSpecular.g, light->mColorSpecular.b, 1));
		SetAttenuation(light->mAttenuationConstant, light->mAttenuationLinear, light->mAttenuationQuadratic);
		SetLookAt(Vector3(light->mDirection.x, light->mDirection.y, light->mDirection.z));
		SetPosition(Vector3(light->mPosition.x, light->mPosition.y, light->mPosition.z));
	}

	LightConverter::~LightConverter()
	{

	}
}
