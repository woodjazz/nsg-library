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
#include "GLES2ModelMaterial.h"
#include "Check.h"
#include "assimp/material.h"


namespace NSG
{
	GLES2ModelMaterial::GLES2ModelMaterial(const aiMaterial* mtl) 
	{
		aiColor4D color;
		float shininess, strength;		

		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &color))
		{
			SetDiffuseColor(Color(color.r, color.g, color.b, color.a));
		}

		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &color))
		{
			SetSpecularColor(Color(color.r, color.g, color.b, color.a));
		}

		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &color))
		{
			SetAmbientColor(Color(color.r, color.g, color.b, color.a));	
		}

		if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &color))
		{

		}

		unsigned int max = 1;

		if(AI_SUCCESS == aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max))
		{
			SetShininess(shininess);
		}

		max = 1;

		if(AI_SUCCESS == aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max))
		{

		}

		int two_sided = 0;

		max = 1;

		if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		{
			EnableCullFace(false);
		}
		else
		{
			EnableCullFace(true);
		}

        unsigned int n = mtl->GetTextureCount(aiTextureType_DIFFUSE);

        if(n > 0)
        {
            aiString path;

            if(AI_SUCCESS == mtl->GetTexture (aiTextureType_DIFFUSE, 0, &path))
            {
            	//PGLES2Texture pTexture(new GLES2Texture(path.C_Str()));
            	std::string filename = path.C_Str();
				const size_t last_slash_idx = filename.find_last_of("\\/");
				if (std::string::npos != last_slash_idx)
				{
				    filename.erase(0, last_slash_idx + 1);
				}

                PGLES2Texture pTexture(new GLES2Texture(filename.c_str()));
            	SetDiffuseTexture(pTexture);
            }
        }

	}

	GLES2ModelMaterial::~GLES2ModelMaterial()
	{

	}
}