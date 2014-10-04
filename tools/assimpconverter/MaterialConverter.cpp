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

#include "MaterialConverter.h"
#include "Material.h"
#include "TextureFile.h"
#include "Check.h"
#include "Util.h"
#include "Pass.h"
#include "Technique.h"
#include "Program.h"
#include "App.h"
#include "assimp/material.h"

namespace NSG
{
	MaterialConverter::MaterialConverter(const aiMaterial* mtl, const std::string& resourcePath)
		: material_(App::this_->CreateMaterial("material"))
	{
		aiString name;
		if (AI_SUCCESS == aiGetMaterialString(mtl, AI_MATKEY_NAME, &name))
		{
			material_->SetName(name.C_Str());

			PTechnique technique(new Technique);
			material_->SetTechnique(technique);
			PPass pass(new Pass);
			technique->Add(pass);

			unsigned int max = 1;
			int shadingModel = 0;
			if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_SHADING_MODEL, &shadingModel, &max))
			{
				switch (shadingModel)
				{
				case aiShadingMode_Flat:
				case aiShadingMode_Gouraud:
					pass->SetProgram(PProgram(new Program("", Program::PER_VERTEX_LIGHTING)));
					break;
				case aiShadingMode_Phong:
				case aiShadingMode_Blinn:
				case aiShadingMode_Toon:
				case aiShadingMode_OrenNayar:
				case aiShadingMode_Minnaert:
				case aiShadingMode_CookTorrance:
				case aiShadingMode_Fresnel:
					pass->SetProgram(PProgram(new Program("", Program::PER_PIXEL_LIGHTING)));
					break;
				default:
					pass->SetProgram(PProgram(new Program("")));
					break;

				}
			}
			else
			{
				pass->SetProgram(PProgram(new Program));
			}

			aiColor4D color;
			if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &color))
			{
				material_->SetDiffuseColor(Color(color.r, color.g, color.b, color.a));
			}

			if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &color))
			{
				material_->SetSpecularColor(Color(color.r, color.g, color.b, color.a));
			}

			if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &color))
			{
				material_->SetAmbientColor(Color(color.r, color.g, color.b, color.a));
			}

			if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &color))
			{
				material_->SetColor(Color(color.r, color.g, color.b, color.a));
			}

			float shininess;
			if (AI_SUCCESS == aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max))
			{
				material_->SetShininess(shininess);
			}

			max = 1;

			float strength;
			if (AI_SUCCESS == aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max))
			{

			}

			int two_sided = 0;

			max = 1;

			if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
			{
				pass->EnableCullFace(false);
			}
			else
			{
				pass->EnableCullFace(true);
			}


			max = 1;
			int blendMode = aiBlendMode_Default;
			if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_BLEND_FUNC, &blendMode, &max))
			{
				if (blendMode == aiBlendMode_Default)
					pass->SetBlendMode(BLEND_MODE::BLEND_ALPHA);
			}

			int wireframe = 0;

			max = 1;

			if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max)) && wireframe)
			{
				pass->SetDrawMode(DrawMode::WIREFRAME);
			}
			else
			{
				pass->SetDrawMode(DrawMode::SOLID);
			}


			unsigned int n = mtl->GetTextureCount(aiTextureType_DIFFUSE);

			if (n > 0)
			{
				aiString path;

				if (AI_SUCCESS == mtl->GetTexture(aiTextureType_DIFFUSE, 0, &path))
				{
					std::string fullPath = path.C_Str();
					ReplaceChar(fullPath, '\\', '/');
					std::string fileName = ExtractFileName(fullPath);
					std::string textureFullPath = resourcePath + "/" + fileName;
					PTexture pTexture(new TextureFile(textureFullPath.c_str()));
					material_->SetTexture0(pTexture);
				}
			}
		}
	}

    MaterialConverter::~MaterialConverter()
    {

    }
}
