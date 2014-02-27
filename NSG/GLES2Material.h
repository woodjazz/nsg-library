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


namespace NSG
{
	class UseMaterial;
	class GLES2Material
	{
	public:
		GLES2Material(PGLES2Texture pTexture, PGLES2Program pProgram);
		~GLES2Material();
		bool IsReady();
		GLuint GetTextCoordAttLocation() { return texcoord_loc_; }
		GLuint GetPositionAttLocation() { return position_loc_; }
		GLuint GetNormalAttLocation() { return normal_loc_; }
		GLuint GetColorAttLocation() { return color_loc_; }
		void SetDiffuseColor(Color diffuse);
	private:
		PGLES2Texture pTexture_;
		PGLES2Program pProgram_;

		GLuint color_difusse_loc_;
		GLuint texture_loc_;
		GLuint texcoord_loc_;
		GLuint position_loc_;
		GLuint normal_loc_;
		GLuint color_loc_;
		GLuint model_inv_transp_loc_;
        GLuint mvp_loc_;
        GLuint m_loc_;
        bool loaded_;

        Color diffuse_;

		friend class UseMaterial;
	};

	typedef std::shared_ptr<GLES2Material> PGLES2Material;

	class UseMaterial
	{
	public:
		UseMaterial(GLES2Material& obj, PNode pNode);
		~UseMaterial();
	private:
		GLES2Material& obj_;
		UseProgram useProgram_;
	};

}