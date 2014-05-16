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

namespace NSG
{
    class GLES2Mesh;
    typedef std::shared_ptr<GLES2Mesh> PGLES2Mesh;

    class GLES2Texture;
    typedef std::shared_ptr<GLES2Texture> PGLES2Texture;

    class GLES2Material;
    typedef std::shared_ptr<GLES2Material> PGLES2Material;

    class GLES2FontAtlasTexture;
    typedef std::shared_ptr<GLES2FontAtlasTexture> PGLES2FontAtlasTexture;

    class GLES2FontAtlasTextureManager;
    typedef std::shared_ptr<GLES2FontAtlasTextureManager> PGLES2FontAtlasTextureManager;

    class App;
	typedef std::shared_ptr<App> PApp;

	struct InternalApp;
	typedef std::shared_ptr<InternalApp> PInternalApp;

	class Behavior;
	typedef std::shared_ptr<Behavior> PBehavior;

	class GLES2BoxMesh;
	typedef std::shared_ptr<GLES2BoxMesh> PGLES2BoxMesh;

	class GLES2Camera;
	typedef std::shared_ptr<GLES2Camera> PGLES2Camera;

	class GLES2CircleMesh;
	typedef std::shared_ptr<GLES2CircleMesh> PGLES2CircleMesh;

	class GLES2EllipseMesh;
	typedef std::shared_ptr<GLES2EllipseMesh> PGLES2EllipseMesh;

	class GLES2Filter;
	typedef std::shared_ptr<GLES2Filter> PGLES2Filter;

	class GLES2FrameColorSelection;
    typedef std::shared_ptr<GLES2FrameColorSelection> PGLES2FrameColorSelection;

    class GLES2FShader;
	typedef std::shared_ptr<GLES2FShader> PGLES2FShader;

	class GLES2IndexBuffer;
	typedef std::shared_ptr<GLES2IndexBuffer> PGLES2IndexBuffer;

	class GLES2Light;
	typedef std::shared_ptr<GLES2Light> PGLES2Light;

	class GLES2ModelMaterial;
	typedef std::shared_ptr<GLES2ModelMaterial> PGLES2ModelMaterial;

	class GLES2ModelMesh;
	typedef std::shared_ptr<GLES2ModelMesh> PGLES2ModelMesh;

	class GLES2PlaneMesh;
	typedef std::shared_ptr<GLES2PlaneMesh> PGLES2PlaneMesh;

	class GLES2Program;
	typedef std::shared_ptr<GLES2Program> PGLES2Program;

	class GLES2RectangleMesh;
	typedef std::shared_ptr<GLES2RectangleMesh> PGLES2RectangleMesh;

	class GLES2Render2Texture;
	typedef std::shared_ptr<GLES2Render2Texture> PGLES2Render2Texture;

	class GLES2RoundedRectangleMesh;
	typedef std::shared_ptr<GLES2RoundedRectangleMesh> PGLES2RoundedRectangleMesh;

	class GLES2SphereMesh;
	typedef std::shared_ptr<GLES2SphereMesh> PGLES2SphereMesh;

	class GLES2StencilMask;
	typedef std::shared_ptr<GLES2StencilMask> PGLES2StencilMask;

	class GLES2Text;
	typedef std::shared_ptr<GLES2Text> PGLES2Text;

	class GLES2VertexBuffer;
	typedef std::shared_ptr<GLES2VertexBuffer> PGLES2VertexBuffer;

	class GLES2VShader;
	typedef std::shared_ptr<GLES2VShader> PGLES2VShader;

	namespace IMGUI
	{
		struct LayoutArea;
		typedef std::shared_ptr<LayoutArea> PLayoutArea;

		class LayoutManager;
		typedef std::shared_ptr<LayoutManager> PLayoutManager;

		struct Skin;
		typedef std::shared_ptr<Skin> PSkin;

		class TextManager;
	    typedef std::shared_ptr<TextManager> PTextManager;

	    struct Context;
	    typedef std::shared_ptr<Context> PContext;

	    struct State;
	    typedef std::shared_ptr<State> PState;
	}

	class Model;
	typedef std::shared_ptr<Model> PModel;

	namespace NaCl 
	{
		class NaClURLLoader;
		typedef std::shared_ptr<NaClURLLoader> PNaClURLLoader;
	}

	class Node;
	typedef std::shared_ptr<Node> PNode;

	class Resource;
	typedef std::shared_ptr<Resource> PResource;

	class SceneNode;
	typedef std::shared_ptr<SceneNode> PSceneNode;



}