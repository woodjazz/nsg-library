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

#include "GLES2Includes.h"
#include "GLES2Texture.h"
#include "GLES2Program.h"
#include "Node.h"
#include <memory>
#include <vector>
#include <map>
#include <string>


namespace NSG
{
	class UseMaterial;
	class GLES2Material
	{
	public:
		GLES2Material();
		~GLES2Material();
		void SetBlendMode(BLEND_MODE mode);
		void EnableDepthTest(bool enable);
		void SetProgram(PGLES2Program pProgram);
		void SetMainTexture(PGLES2Texture pTexture);
        PGLES2Texture GetMainTexture() const { return pTexture_; }
		bool IsReady();
		GLuint GetTextCoordAttLocation() { return texcoord_loc_; }
		GLuint GetPositionAttLocation() { return position_loc_; }
		GLuint GetNormalAttLocation() { return normal_loc_; }
		GLuint GetMVPAttLocation() { return mvp_loc_; }
		void SetDiffuseColor(Color diffuse) { diffuse_ = diffuse; }
		Color GetDiffuseColor() const { return diffuse_; }
		void SetSpecularColor(Color specular) { specular_ = specular; }
		void SetShininess(float shininess) { shininess_=shininess; }
		float GetShininess() const { return shininess_; }
		void SetUniformValue(const char* name, int value);
		int GetUniformValue(const char* name) const;
		void Render(bool solid, Node* pNode, GLES2Mesh* pMesh);

	private:
		PGLES2Texture pTexture_;
		PGLES2Program pProgram_;

		GLuint color_scene_ambient_loc_;
		GLuint color_ambient_loc_;
		GLuint color_diffuse_loc_;
		GLuint color_specular_loc_;
		GLuint shininess_loc_;
		GLuint texture_loc_;
		GLuint texcoord_loc_;
		GLuint position_loc_;
		GLuint normal_loc_;
		GLuint model_inv_transp_loc_;
		GLuint v_inv_loc_;
        GLuint mvp_loc_;
        GLuint m_loc_;

        struct LightLoc
        {
        	GLuint type_loc;
        	GLuint position_loc;
	        GLuint diffuse_loc;
	        GLuint specular_loc;
	        GLuint constantAttenuation_loc;
	        GLuint linearAttenuation_loc;
	        GLuint quadraticAttenuation_loc;
	        GLuint spotCutoff_loc;
	        GLuint spotExponent_loc;
	        GLuint spotDirection_loc;
	    };

	    typedef std::vector<LightLoc> LightsLoc;
	    LightsLoc lightsLoc_;

        bool loaded_;
        Color ambient_;
        Color diffuse_;
        Color specular_;
        float shininess_;

        bool hasLights_;

        BLEND_MODE blendMode_;
        bool enableDepthTest_;

		friend class UseMaterial;
	};

	class UseMaterial
	{
	public:
		UseMaterial(GLES2Material& obj, Node* pNode);
		~UseMaterial();
	private:
		GLES2Material& obj_;
		UseProgram useProgram_;
	};

}