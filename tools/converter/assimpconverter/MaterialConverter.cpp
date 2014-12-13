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
#include "Texture.h"
#include "Check.h"
#include "Util.h"
#include "Pass.h"
#include "Technique.h"
#include "App.h"
#include "Path.h"
#include "Util.h"
#include "TextureFileManager.h"
#include "assimp/material.h"

namespace NSG
{
    MaterialConverter::MaterialConverter(const aiMaterial* mtl, const std::string& resourcePath)
		: program_(App::this_->GetOrCreateProgram(GetUniqueName())),
		  flags_((int)ProgramFlag::NONE),
          resourcePath_(resourcePath),
          mtl_(mtl)
    {
		std::string materialName;
		{
			aiString name;
			if (AI_SUCCESS == aiGetMaterialString(mtl, AI_MATKEY_NAME, &name))
			{
				materialName = name.C_Str();
			}
			else
			{
				materialName  = "NSGMaterialConverter";
				static int counter = 0;
				std::stringstream ss;
				ss << counter++;
				materialName += ss.str();
			}
		}

		material_ = App::this_->GetOrCreateMaterial(materialName);

        PTechnique technique(new Technique);
        material_->SetTechnique(technique);
        PPass pass(new Pass);
        technique->Add(pass);
        pass->SetProgram(program_);

        unsigned int max = 1;
        int shadingModel = 0;
        if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_SHADING_MODEL, &shadingModel, &max))
        {
            switch (shadingModel)
            {
                case aiShadingMode_Flat:
                case aiShadingMode_Gouraud:
					flags_ |= (int)ProgramFlag::PER_VERTEX_LIGHTING;
                    program_->SetFlags(flags_);
                    break;
                case aiShadingMode_Phong:
                case aiShadingMode_Blinn:
                case aiShadingMode_Toon:
                case aiShadingMode_OrenNayar:
                case aiShadingMode_Minnaert:
                case aiShadingMode_CookTorrance:
                case aiShadingMode_Fresnel:
					flags_ |= (int)ProgramFlag::PER_PIXEL_LIGHTING;
                    program_->SetFlags(flags_);
                    break;
                default:
                    break;
            }
        }
		else
		{
			program_->SetFlags((int)ProgramFlag::PER_PIXEL_LIGHTING);
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

        SetDiffuseMap();
        SetNormalMap();
        SetLightMap();
    }

    void MaterialConverter::SetDiffuseMap()
    {
        unsigned int n = mtl_->GetTextureCount(aiTextureType_DIFFUSE);

        if (n > 0)
        {
            if (n > 1)
            {
                TRACE_LOG("File constains more than one diffuse map per material. Material name = " << material_->GetName() << " #diffuse maps = " << n);
            }

            aiString aPath;
            aiTextureMapping mapping(aiTextureMapping_UV);
            unsigned int uvindex(0);
            float blend(1);
            aiTextureOp op(aiTextureOp_Add);
            aiTextureMapMode mapmode[3];

			if (AI_SUCCESS == mtl_->GetTexture(aiTextureType_DIFFUSE, 0, &aPath, &mapping, &uvindex, &blend, &op, mapmode))
            {
                Path path(aPath.C_Str());
                if (mapping == aiTextureMapping_UV)
                {
                    if (uvindex == 0)
                    {
                        material_->SetDiffuseMap(CreateTexture(path));
                    }
                    else if (uvindex == 1)
                    {
                        TRACE_LOG("Detected diffuse map using uv1 => assuming it is a lightmap");
                        material_->SetLightMap(CreateTexture(path));
                    }
                    else
                    {
                        TRACE_LOG("Material name = " << material_->GetName() << " with texture " << path.GetFilePath() << " uses uv index greater than 1. Only uv0 and uv1 are supported!!!");
                    }
                }
                else
                {
                    TRACE_LOG("Material name = " << material_->GetName() << " with texture " << path.GetFilePath() << " does not use UV mapping!!!");
                }
            }
        }
    }

    void MaterialConverter::SetNormalMap()
    {
        unsigned int n = mtl_->GetTextureCount(aiTextureType_NORMALS);

        if (n > 0)
        {
            if (n > 1)
            {
                TRACE_LOG("File constains more than one normal map per material. Material name = " << material_->GetName() << " #normal maps = " << n);
            }

            aiString aPath;
            aiTextureMapping mapping(aiTextureMapping_UV);
            unsigned int uvindex(0);
            float blend(1);
            aiTextureOp op(aiTextureOp_Add);
            aiTextureMapMode mapmode[3];

            if (AI_SUCCESS == mtl_->GetTexture(aiTextureType_NORMALS, 0, &aPath, &mapping, &uvindex, &blend, &op, mapmode))
            {
                Path path(aPath.C_Str());
                if (mapping == aiTextureMapping_UV)
                {
                    if (uvindex == 0)
                    {
                        material_->SetNormalMap(CreateTexture(path));
                    }
                    else if (uvindex == 1)
                    {
                        TRACE_LOG("Detected normal map using uv1. Normal map only works with uv0!!!");
                    }
                    else
                    {
                        TRACE_LOG("Material name = " << material_->GetName() << " with texture " << path.GetFilePath() << " uses uv index greater than 1. Only uv0 and uv1 are supported!!!");
                    }
                }
                else
                {
                    TRACE_LOG("Material name = " << material_->GetName() << " with texture " << path.GetFilePath() << " does not use UV mapping!!!");
                }
            }
        }
    }

    void MaterialConverter::SetLightMap()
    {
        unsigned int n = mtl_->GetTextureCount(aiTextureType_LIGHTMAP);

        if (n > 0)
        {
            if (n > 1)
            {
                TRACE_LOG("File constains more than one light map per material. Material name = " << material_->GetName() << " #light maps = " << n);
            }

            aiString aPath;
            aiTextureMapping mapping(aiTextureMapping_UV);
            unsigned int uvindex(0);
            float blend(1);
            aiTextureOp op(aiTextureOp_Add);
            aiTextureMapMode mapmode[3];

            if (AI_SUCCESS == mtl_->GetTexture(aiTextureType_LIGHTMAP, 0, &aPath, &mapping, &uvindex, &blend, &op, mapmode))
            {
                Path path(aPath.C_Str());
                if (mapping == aiTextureMapping_UV)
                {
                    if (uvindex == 1)
                    {
                        material_->SetLightMap(CreateTexture(path));
                    }
                    else if (uvindex == 0)
                    {
                        TRACE_LOG("Detected light map using uv0. Light map only works with uv1!!!");
                    }
                    else
                    {
                        TRACE_LOG("Material name = " << material_->GetName() << " with texture " << path.GetFilePath() << " uses uv index greater than 1. Only uv0 and uv1 are supported!!!");
                    }
                }
                else
                {
                    TRACE_LOG("Material name = " << material_->GetName() << " with texture " << path.GetFilePath() << " does not use UV mapping!!!");
                }
            }
        }
    }


    PTexture MaterialConverter::CreateTexture(const Path& path)
    {
        std::string textureFilePath;
        if (path.IsPathRelative())
        {
            if(path.GetPath().empty())
                textureFilePath = resourcePath_ + "/" + path.GetFilename();
            else
                textureFilePath = resourcePath_ + "/" + path.GetPath() + "/" + path.GetFilename();
        }
        else
            textureFilePath = path.GetFilePath();

		return TextureFileManager::this_->GetOrCreate(textureFilePath);
    }

    MaterialConverter::~MaterialConverter()
    {

    }
}
