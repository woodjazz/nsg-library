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
#include <vector>

namespace NSG
{
	class Frustum;
	typedef std::shared_ptr<Frustum> PFrustum;
	
	class Audio;
	typedef std::unique_ptr<Audio> PAudio;

	class Music;
	typedef std::unique_ptr<Music> PMusic;
	
	class Sound;
	typedef std::shared_ptr<Sound> PSound;

	class Buffer;
	typedef std::shared_ptr<Buffer> PBuffer;

	class BufferManager;
	typedef std::unique_ptr<BufferManager> PBufferManager;

	struct Context;
	typedef std::unique_ptr<Context> PContext;
	
	struct AppConfiguration;
	typedef std::shared_ptr<AppConfiguration> PAppConfiguration;
	
	class AppStatistics;
	typedef std::unique_ptr<AppStatistics> PAppStatistics;

	class Technique;
	typedef std::shared_ptr<Technique> PTechnique;

	class PassFilter;
	typedef std::shared_ptr<PassFilter> PPassFilter;

	class Pass2Texture;
	typedef std::shared_ptr<Pass2Texture> PPass2Texture;

	class Pass;
	typedef std::shared_ptr<Pass> PPass;

	class Pass2Stencil;
	typedef std::shared_ptr<Pass2Stencil> PPass2Stencil;

	typedef std::vector<PPass> PASSES;

	namespace FSM
	{
		class Machine;
		typedef std::shared_ptr<Machine> PMachine;
	}
	
	class ResourceFile;
	typedef std::shared_ptr<ResourceFile> PResourceFile;
	
	class ShowTexture;
	typedef std::shared_ptr<ShowTexture> PShowTexture;

    class Mesh;
    typedef std::shared_ptr<Mesh> PMesh;

    class Texture;
    typedef std::shared_ptr<Texture> PTexture;

    class TextureFile;
    typedef std::shared_ptr<TextureFile> PTextureFile;
    
    class Material;
    typedef std::shared_ptr<Material> PMaterial;

    class FontAtlasTexture;
    typedef std::shared_ptr<FontAtlasTexture> PFontAtlasTexture;

    class FontAtlasTextureManager;
    typedef std::unique_ptr<FontAtlasTextureManager> PFontAtlasTextureManager;

    class App;
	typedef std::unique_ptr<App> PApp;

	struct InternalApp;
	typedef std::unique_ptr<InternalApp> PInternalApp;

	class Behavior;
	typedef std::shared_ptr<Behavior> PBehavior;

	class BoxMesh;
	typedef std::shared_ptr<BoxMesh> PBoxMesh;

	class Camera;
	typedef std::shared_ptr<Camera> PCamera;

	class CircleMesh;
	typedef std::shared_ptr<CircleMesh> PCircleMesh;

	class EllipseMesh;
	typedef std::shared_ptr<EllipseMesh> PEllipseMesh;

	class Filter;
	typedef std::shared_ptr<Filter> PFilter;

	class FilterBlur;
	typedef std::shared_ptr<FilterBlur> PFilterBlur;

	class FrameColorSelection;
    typedef std::shared_ptr<FrameColorSelection> PFrameColorSelection;

    class FragmentShader;
	typedef std::shared_ptr<FragmentShader> PFragmentShader;

	class IndexBuffer;
	typedef std::shared_ptr<IndexBuffer> PIndexBuffer;

	class Light;
	typedef std::shared_ptr<Light> PLight;

	class ModelMaterial;
	typedef std::shared_ptr<ModelMaterial> PModelMaterial;

	class ModelMesh;
	typedef std::shared_ptr<ModelMesh> PModelMesh;

	class PlaneMesh;
	typedef std::shared_ptr<PlaneMesh> PPlaneMesh;

	class Program;
	typedef std::shared_ptr<Program> PProgram;

	class ProgramPerVertex;
	typedef std::shared_ptr<ProgramPerVertex> PProgramPerVertex;

	class ProgramUnlit;
	typedef std::shared_ptr<ProgramUnlit> PProgramUnlit;

	class ProgramWhiteColor;
	typedef std::shared_ptr<ProgramWhiteColor> PProgramWhiteColor;

	class RectangleMesh;
	typedef std::shared_ptr<RectangleMesh> PRectangleMesh;

	class Render2Texture;
	typedef std::shared_ptr<Render2Texture> PRender2Texture;

	class RoundedRectangleMesh;
	typedef std::shared_ptr<RoundedRectangleMesh> PRoundedRectangleMesh;

	class SphereMesh;
	typedef std::shared_ptr<SphereMesh> PSphereMesh;

	class StencilMask;
	typedef std::shared_ptr<StencilMask> PStencilMask;

	class TextMesh;
	typedef std::shared_ptr<TextMesh> PTextMesh;

	class VertexBuffer;
	typedef std::shared_ptr<VertexBuffer> PVertexBuffer;

	class VertexShader;
	typedef std::shared_ptr<VertexShader> PVertexShader;

	namespace IMGUI
	{
		struct LayoutArea;
		typedef std::shared_ptr<LayoutArea> PLayoutArea;

		class LayoutManager;
		typedef std::unique_ptr<LayoutManager> PLayoutManager;

		struct Skin;
		typedef std::shared_ptr<Skin> PSkin;

		class TextManager;
	    typedef std::unique_ptr<TextManager> PTextManager;

	    struct Context;
	    typedef std::unique_ptr<Context> PContext;

	    struct State;
	    typedef std::shared_ptr<State> PState;

	    class Area;
	    typedef std::shared_ptr<Area> PArea;

	    class Title;
	    typedef std::shared_ptr<Title> PTitle;

	    class Window;
	    typedef std::shared_ptr<Window> PWindow;

		struct WindowManager;
		typedef std::shared_ptr<WindowManager> PWindowManager;

		struct Style;
		typedef std::shared_ptr<Style> PStyle;

		struct ButtonStyle;
		typedef std::shared_ptr<ButtonStyle> PButtonStyle;

		struct LabelStyle;
		typedef std::shared_ptr<LabelStyle> PLabelStyle;

		struct TextStyle;
		typedef std::shared_ptr<TextStyle> PTextStyle;

		struct TitleStyle;
		typedef std::shared_ptr<TitleStyle> PTitleStyle;

		struct WindowStyle;
		typedef std::shared_ptr<WindowStyle> PWindowStyle;

		struct AreaStyle;
		typedef std::shared_ptr<AreaStyle> PAreaStyle;

		struct SliderStyle;
		typedef std::shared_ptr<SliderStyle> PSliderStyle;

		struct SizerStyle;
		typedef std::shared_ptr<SizerStyle> PSizerStyle;

		struct LineStyle;
		typedef std::shared_ptr<LineStyle> PLineStyle;

		struct CheckButtonStyle;
		typedef std::shared_ptr<CheckButtonStyle> PCheckButtonStyle;
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

	class Keyboard;
	typedef std::unique_ptr<Keyboard> PKeyboard;
}