/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

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
    class Worker;
    typedef std::shared_ptr<Worker> PWorker;

    class Window;
    typedef std::shared_ptr<Window> PWindow;
    typedef std::weak_ptr<Window> PWeakWindow;

    class RigidBody;
    typedef std::shared_ptr<RigidBody> PRigidBody;

    class Character;
    typedef std::shared_ptr<Character> PCharacter;

    class Shape;
    typedef std::shared_ptr<Shape> PShape;
    typedef std::weak_ptr<Shape> PWeakShape;

    class PhysicsWorld;
    typedef std::shared_ptr<PhysicsWorld> PPhysicsWorld;
    typedef std::weak_ptr<PhysicsWorld> PWeakPhysicsWorld;

    class Skeleton;
    typedef std::shared_ptr<Skeleton> PSkeleton;
    typedef std::weak_ptr<Skeleton> PWeakSkeleton;

    class AnimationState;
    typedef std::shared_ptr<AnimationState> PAnimationState;

    class Animation;
    typedef std::shared_ptr<Animation> PAnimation;
    typedef std::weak_ptr<Animation> PWeakAnimation;

    class AnimationController;
    typedef std::shared_ptr<AnimationController> PAnimationController;

    struct AnimationControl;
    typedef std::shared_ptr<AnimationControl> PAnimationControl;

    class PointOnSphere;
    typedef std::shared_ptr<PointOnSphere> PPointOnSphere;

    class Ray;
    typedef std::shared_ptr<Ray> PRay;

    class Octree;
    typedef std::shared_ptr<Octree> POctree;

    struct BoundingBox;
    typedef std::shared_ptr<BoundingBox> PBoundingBox;

    class Scene;
    typedef std::shared_ptr<Scene> PScene;
    typedef std::weak_ptr<Scene> PWeakScene;

    class Graphics;
    typedef std::shared_ptr<Graphics> PGraphics;

    class Frustum;
    typedef std::shared_ptr<Frustum> PFrustum;

    class Audio;
    typedef std::unique_ptr<Audio> PAudio;

    class Music;
    typedef std::unique_ptr<Music> PMusic;

    class Sound;
    typedef std::shared_ptr<Sound> PSound;
    typedef std::weak_ptr<Sound> PWeakSound;

    class Buffer;
    typedef std::shared_ptr<Buffer> PBuffer;

    struct Context;
    typedef std::unique_ptr<Context> PContext;

    struct AppConfiguration;
    typedef std::shared_ptr<AppConfiguration> PAppConfiguration;

    class Technique;
    typedef std::shared_ptr<Technique> PTechnique;
    typedef std::weak_ptr<Technique> PWeakTechnique;

    class FilterBlend;
    typedef std::shared_ptr<FilterBlend> PFilterBlend;

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

    class Batch;
    typedef std::shared_ptr<Batch> PBatch;
    typedef std::weak_ptr<Batch> PWeakBatch;

    class ResourceFile;
    typedef std::shared_ptr<ResourceFile> PResourceFile;
    typedef std::weak_ptr<ResourceFile> PWeakResourceFile;

    class ResourceXMLNode;
    typedef std::shared_ptr<ResourceXMLNode> PResourceXMLNode;
    typedef std::weak_ptr<ResourceXMLNode> PWeakResourceXMLNode;

    class Resource;
    typedef std::shared_ptr<Resource> PResource;
    typedef std::weak_ptr<Resource> PWeakResource;

    class ShowTexture;
    typedef std::shared_ptr<ShowTexture> PShowTexture;

    class Mesh;
    typedef std::shared_ptr<Mesh> PMesh;
    typedef std::weak_ptr<Mesh> PWeakMesh;

    class Texture;
    typedef std::shared_ptr<Texture> PTexture;

    class Texture2D;
    typedef std::shared_ptr<Texture2D> PTexture2D;


    class Material;
    typedef std::shared_ptr<Material> PMaterial;
    typedef std::weak_ptr<Material> PWeakMaterial;

    class FontAtlas;
    typedef std::shared_ptr<FontAtlas> PFontAtlas;
    typedef std::weak_ptr<FontAtlas> PWeakFontAtlas;

    class FontAtlasTextureManager;
    typedef std::unique_ptr<FontAtlasTextureManager> PFontAtlasTextureManager;

    class App;
    typedef std::unique_ptr<App> PApp;

    struct InternalApp;
    typedef std::unique_ptr<InternalApp> PInternalApp;

    struct Behavior;
    typedef std::shared_ptr<Behavior> PBehavior;

    class CameraControl;
    typedef std::shared_ptr<CameraControl> PCameraControl;

    class FollowCamera;
    typedef std::shared_ptr<FollowCamera> PFollowCamera;
    
    class PlayerControl;
    typedef std::shared_ptr<PlayerControl> PPlayerControl;

    class ShadowCamera;
    typedef std::shared_ptr<ShadowCamera> PShadowCamera;

    class BoxMesh;
    typedef std::shared_ptr<BoxMesh> PBoxMesh;

    class Camera;
    typedef std::shared_ptr<Camera> PCamera;
    typedef std::weak_ptr<Camera> PWeakCamera;

    class CircleMesh;
    typedef std::shared_ptr<CircleMesh> PCircleMesh;

    class LinesMesh;
    typedef std::shared_ptr<LinesMesh> PLinesMesh;

    class FrustumMesh;
    typedef std::shared_ptr<FrustumMesh> PFrustumMesh;

    class EllipseMesh;
    typedef std::shared_ptr<EllipseMesh> PEllipseMesh;

    class Filter;
    typedef std::shared_ptr<Filter> PFilter;

    class FilterBlur;
    typedef std::shared_ptr<FilterBlur> PFilterBlur;

    class FrameBuffer;
    typedef std::shared_ptr<FrameBuffer> PFrameBuffer;

    class FragmentShader;
    typedef std::shared_ptr<FragmentShader> PFragmentShader;

    class IndexBuffer;
    typedef std::shared_ptr<IndexBuffer> PIndexBuffer;

    class VertexArrayObj;
    typedef std::shared_ptr<VertexArrayObj> PVertexArrayObj;

    class Light;
    typedef std::shared_ptr<Light> PLight;
    typedef std::weak_ptr<Light> PWeakLight;

    class PlaneMesh;
    typedef std::shared_ptr<PlaneMesh> PPlaneMesh;

    class Program;
    typedef std::shared_ptr<Program> PProgram;
    typedef std::weak_ptr<Program> PWeakProgram;

    class RectangleMesh;
    typedef std::shared_ptr<RectangleMesh> PRectangleMesh;

    class TriangleMesh;
    typedef std::shared_ptr<TriangleMesh> PTriangleMesh;

    class RoundedRectangleMesh;
    typedef std::shared_ptr<RoundedRectangleMesh> PRoundedRectangleMesh;

    class SphereMesh;
    typedef std::shared_ptr<SphereMesh> PSphereMesh;

    class CylinderMesh;
    typedef std::shared_ptr<CylinderMesh> PCylinderMesh;

    class IcoSphereMesh;
    typedef std::shared_ptr<IcoSphereMesh> PIcoSphereMesh;

    class ModelMesh;
    typedef std::shared_ptr<ModelMesh> PModelMesh;

    class ConverterMesh;
    typedef std::shared_ptr<ConverterMesh> PConverterMesh;

    class StencilMask;
    typedef std::shared_ptr<StencilMask> PStencilMask;

    class TextMesh;
    typedef std::shared_ptr<TextMesh> PTextMesh;
    typedef std::weak_ptr<TextMesh> PWeakTextMesh;

    class VertexBuffer;
    typedef std::shared_ptr<VertexBuffer> PVertexBuffer;

    class InstanceBuffer;
    typedef std::shared_ptr<InstanceBuffer> PInstanceBuffer;

    class VertexShader;
    typedef std::shared_ptr<VertexShader> PVertexShader;

    class Node;
    typedef std::shared_ptr<Node> PNode;
    typedef std::weak_ptr<Node> PWeakNode;

    class Bone;
    typedef std::shared_ptr<Bone> PBone;
    typedef std::weak_ptr<Bone> PWeakBone;

    class Resource;
    typedef std::shared_ptr<Resource> PResource;

    class SceneNode;
    typedef std::shared_ptr<SceneNode> PSceneNode;
    typedef std::weak_ptr<SceneNode> PWeakSceneNode;

    class Keyboard;
    typedef std::unique_ptr<Keyboard> PKeyboard;

    class ParticleSystem;
    typedef std::shared_ptr<ParticleSystem> PParticleSystem;
    typedef std::weak_ptr<ParticleSystem> PWeakParticleSystem;

    class Particle;
    typedef std::shared_ptr<Particle> PParticle;

    class QuadMesh;
    typedef std::shared_ptr<QuadMesh> PQuadMesh;

    class Object;
    typedef std::shared_ptr<Object> PObject;
    typedef std::weak_ptr<Object> PWeakObject;

    class Image;
    typedef std::shared_ptr<Image> PImage;

    class Renderer;
    typedef std::shared_ptr<Renderer> PRenderer;

    class Plane;
    typedef std::shared_ptr<Plane> PPlane;

    class LoaderXML;
    typedef std::shared_ptr<LoaderXML> PLoaderXML;

    class LoaderXMLNode;
    typedef std::shared_ptr<LoaderXMLNode> PLoaderXMLNode;

    class Engine;
    typedef std::shared_ptr<Engine> PEngine;

    class HTTPRequest;
    typedef std::shared_ptr<HTTPRequest> PHTTPRequest;

    class GUI;
    typedef std::shared_ptr<GUI> PGUI;

    class Font;
    typedef std::shared_ptr<Font> PFont;
    typedef std::weak_ptr<Font> PWeakFont;

    class DebugRenderer;
    typedef std::shared_ptr<DebugRenderer> PDebugRenderer;

    class ShadowMapDebug;
    typedef std::shared_ptr<ShadowMapDebug> PShadowMapDebug;

    class Overlay;
    typedef std::shared_ptr<Overlay> POverlay;
}
