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
#include "Types.h"
#include <vector>

namespace NSG
{
	typedef std::vector<MeshNode> MESHNODES;

	class Pass
	{
	public:
		Pass();
		virtual ~Pass();
		void SetBlendMode(BLEND_MODE mode);
		void EnableColorBuffer(bool enable);
		void EnableDepthBuffer(bool enable);
		void EnableDepthTest(bool enable);
		void EnableStencilTest(bool enable);
		void SetStencilMask(GLuint mask);
		void SetStencilOp(GLenum sfail, GLenum dpfail, GLenum dppass);
		void SetStencilFunc(GLenum func, GLint ref, GLuint mask);
		enum Mode {SOLID, WIREFRAME};
		void SetDrawMode(Mode mode) { drawMode_ = mode; }
		void SetNode(int idx, PNode node);
        void SetAll(PNode node);
        void SetAll(Node* node);
		void Set(PMaterial material);
		void Set(Material* pMaterial);
		void Add(PNode node, PMesh mesh);
		void Add(Node* node, PMesh mesh);
		void ClearMeshNodes();
		virtual bool Render();
		PMesh GetMesh(int idx) const;
		const MESHNODES& GetMeshNodes() const { return meshNodes_; }
		PMaterial GetMaterial() const { return material_; }
	protected:
        BLEND_MODE blendMode_;
        bool enableDepthTest_;
        bool enableStencilTest_;
        GLuint stencilMask_;
        GLenum sfailStencilOp_;
        GLenum dpfailStencilOp_;
        GLenum dppassStencilOp_;
        GLenum stencilFunc_;
        GLint stencilRefValue_;
        GLuint stencilMaskValue_;
        bool enableColorBuffer_;
        bool enableDepthBuffer_;

		PMaterial material_;
		MESHNODES meshNodes_;
		Mode drawMode_;
		friend class Technique;

	};
}