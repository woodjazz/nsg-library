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
#include "Texture.h"
#include "Check.h"
#include "Util.h"
#include "Constants.h"
#include "Pass.h"
#include "Scene.h"
#include "Light.h"
#include "Technique.h"
#include "assimp/light.h"

namespace NSG
{
	LightConverter::LightConverter(const aiLight* light, Light* lightNode)
    {
        switch (light->mType)
        {
            case aiLightSource_DIRECTIONAL:
				lightNode->SetType(LightType::DIRECTIONAL);
                break;
            case aiLightSource_POINT:
				lightNode->SetType(LightType::POINT);
                break;
            case aiLightSource_SPOT:
                {
                    float spotCutOff = glm::degrees(light->mAngleInnerCone);
					lightNode->SetSpotCutOff(spotCutOff);
					lightNode->SetType(LightType::SPOT);
                    break;
                }
            default:
                CHECK_ASSERT(false, __FILE__, __LINE__);
        }

		lightNode->SetAmbientColor(Color(light->mColorAmbient.r, light->mColorAmbient.g, light->mColorAmbient.b, 1));
		lightNode->SetDiffuseColor(Color(light->mColorDiffuse.r, light->mColorDiffuse.g, light->mColorDiffuse.b, 1));
		lightNode->SetSpecularColor(Color(light->mColorSpecular.r, light->mColorSpecular.g, light->mColorSpecular.b, 1));
		lightNode->SetAttenuation(light->mAttenuationConstant, light->mAttenuationLinear, light->mAttenuationQuadratic);
		Node node;
		node.SetLocalLookAt(Vector3(light->mDirection.x, light->mDirection.y, light->mDirection.z));
		lightNode->Rotate(node.GetOrientation());
		lightNode->Translate(Vector3(light->mPosition.x, light->mPosition.y, light->mPosition.z));
    }

    LightConverter::~LightConverter()
    {

    }
}
