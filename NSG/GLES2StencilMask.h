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
#include <memory>
#include "GLES2Includes.h"
#include "GLES2Material.h"
#include "GLES2GPUObject.h"

namespace NSG
{
	class Node;
	class GLES2Mesh;
	class GLES2StencilMask : public GLES2GPUObject
	{
	public:
		GLES2StencilMask();
		~GLES2StencilMask();
		void Begin();
		void Render(Node* pNode, GLES2Mesh* pMesh);
		void End();
		virtual bool IsValid();
		virtual void AllocateResources();
		virtual void ReleaseResources();
	private:
		GLboolean save_color_mask_[4];
		GLboolean save_depth_mask_;
		PGLES2Program pProgram_;
        GLuint position_loc_;
        GLuint mvp_loc_;
        bool enabled_;
	};

    typedef std::shared_ptr<GLES2StencilMask> PGLES2StencilMask;
}