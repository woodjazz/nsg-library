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
#include "IMGUISkin.h"
#include "GLES2RoundedRectangleMesh.h"

static const char* vShader = STRINGIFY(
	uniform mat4 u_mvp;
	attribute vec4 a_position;
	attribute vec2 a_texcoord;
	varying vec4 v_position;
	varying vec2 v_texcoord;
	
	void main()
	{
		gl_Position = u_mvp * vec4(a_position.xyz, 1);
		v_position = a_position;
		v_texcoord = a_texcoord;
	}
);

static const char* fShader = STRINGIFY(
	struct Material
	{
		vec4 diffuse;
		float shininess;
	};
	uniform Material u_material;
	uniform sampler2D u_texture0;
	varying vec4 v_position;
	varying vec2 v_texcoord;

	void main()
	{
		float factor = u_material.shininess - abs(v_position.y);

        if(u_material.shininess < 0.0)
            factor = abs(v_position.y) + u_material.shininess;

		gl_FragColor = texture2D(u_texture0, v_texcoord) * u_material.diffuse * vec4(factor, factor, factor, 1.0);
	}
);

static const char* fShaderBorder = STRINGIFY(
	struct Material
	{
		vec4 diffuse;
	};

	uniform Material u_material;

	void main()
	{
		gl_FragColor = u_material.diffuse;
	}
);


namespace NSG 
{
	namespace IMGUI
	{
		Skin::Skin() 
		: alphaFactor(1),
		fontFile("font/FreeSans.ttf"),
		fontSize(18),
		textMaxLength(std::numeric_limits<int>::max()),
		fillEnabled(true),
		drawBorder(true),
        pActiveMaterial(new GLES2Material()),
        pNormalMaterial(new GLES2Material()),
        pHotMaterial(new GLES2Material()),
        pBorderMaterial(new GLES2Material()),
		pMesh(new GLES2RoundedRectangleMesh(0.5f, 2, 2, 64, GL_STATIC_DRAW))
		{
			pActiveMaterial->SetBlendMode(ALPHA);
			pActiveMaterial->EnableDepthTest(false);
			pNormalMaterial->SetBlendMode(ALPHA);
			pNormalMaterial->EnableDepthTest(false);
			pHotMaterial->SetBlendMode(ALPHA);
			pHotMaterial->EnableDepthTest(false);
			pBorderMaterial->SetBlendMode(ALPHA);
			pBorderMaterial->EnableDepthTest(false);

            PGLES2Program pProgram(new GLES2Program(vShader, fShader));
            pActiveMaterial->SetProgram(pProgram);
			pActiveMaterial->SetDiffuseColor(Color(0,1,0,0.7f));
			//pActiveMaterial->SetDiffuseColor(Color(1,1,1,0.7f));
			pActiveMaterial->SetShininess(0);

            pNormalMaterial->SetProgram(pProgram);
			pNormalMaterial->SetDiffuseColor(Color(0.5f,0.5f,0.5f,0.5f));
			pNormalMaterial->SetShininess(0);

			pHotMaterial->SetProgram(pProgram);
			pHotMaterial->SetDiffuseColor(Color(1,0.5f,0.5f,0.7f));
			pHotMaterial->SetShininess(0);

			PGLES2Program pBorderProgram(new GLES2Program(vShader, fShaderBorder));
			pBorderMaterial->SetProgram(pBorderProgram);
			pBorderMaterial->SetDiffuseColor(Color(1,1,1,1));
		}

		Skin::Skin(const Skin& obj)
		: alphaFactor(obj.alphaFactor),
		fontFile(obj.fontFile),
		fontSize(obj.fontSize),
		textMaxLength(obj.textMaxLength),
		fillEnabled(obj.fillEnabled),
		drawBorder(obj.drawBorder),
		pActiveMaterial(obj.pActiveMaterial),
		pNormalMaterial(obj.pNormalMaterial),
		pHotMaterial(obj.pHotMaterial),
		pBorderMaterial(obj.pBorderMaterial),
		pMesh(obj.pMesh)
		{
		}
	}
}