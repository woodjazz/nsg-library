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

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/epsilon.hpp"
#include "glm/gtc/matrix_access.hpp"
#include "glm/gtx/spline.hpp"
#include "glm/gtx/io.hpp"
#include "glm/gtx/norm.hpp"
#include "SharedPointers.h"
#include "GLES2Includes.h"
#include "FlagSet.h"
#include "SignalSlots.h"
#include <string>
#include <chrono>
#include <set>

#define STRINGIFY(S) #S

namespace pugi
{
    class xml_node;
    class xml_document;
}

namespace NSG
{
    class Path;

    typedef glm::highp_ivec4 Recti;
    typedef glm::highp_ivec4 Vector4i;
    typedef glm::vec4 Rect;
    typedef glm::vec4 Vertex4;
    typedef glm::vec4 Vector4;
    typedef glm::vec3 Vertex3;
    typedef glm::vec3 Vector3;
    typedef glm::vec2 Vertex2;
    typedef glm::vec2 Vector2;
    typedef glm::quat Quaternion;
    typedef glm::mat4 Matrix4;
    typedef glm::mat3 Matrix3;
    typedef glm::vec4 Color;

    enum class LightType 
	{
		POINT, 
		DIRECTIONAL, 
		SPOT,
		MAX_INDEX
	};

    enum class BLEND_MODE
    {
        NONE,
        ALPHA,
        MULTIPLICATIVE,
        ADDITIVE,
		MAX_INDEX
    };

    enum class TextureBlend
    {
        NONE,
        MIX,
        MUL,
        ADD,
        SUB,
        DIV,
        DARK,
        DIFF,
        LIGHT,
        SCREEN,
        OVERLAY,
        BLEND_HUE,
        BLEND_SAT,
        BLEND_VAL,
        BLEND_COLOR,
		MAX_INDEX
    };

    enum class TextureType
    {
        UNKNOWN,
        COL,
        NORM,
        COLSPEC,
        COLMIR,
        VARS,
        REF,
        SPEC,
        EMIT,
        ALPHA,
        HAR,
        RAYMIRR,
        TRANSLU,
        AMB,
        DISPLACE,
        WARP,
        LAYER,
		MAX_INDEX
    };

    typedef enum {LEFT_ALIGNMENT, CENTER_ALIGNMENT, RIGHT_ALIGNMENT} HorizontalAlignment;
    typedef enum {TOP_ALIGNMENT, MIDDLE_ALIGNMENT, BOTTOM_ALIGNMENT} VerticalAlignment;

    typedef GLushort IdType;

    namespace IMGUI
    {
        enum class LayoutType
        {
            WINDOW,
            VERTICAL,
            HORIZONTAL,
            CONTROL,
            SPACER,
            LINE
        };

        enum class IdsTypes : IdType
        {
            IMGUI_UNKNOWN_ID,
            IMGUI_MAIN_WINDOW_ID,
            IMGUI_VERTICAL_SLIDER_ID,
            IMGUI_HORIZONTAL_SLIDER_ID,
            IMGUI_FIRST_VALID_ID
        } ;

        enum class SizerType
        {
            LEFT_TOP_SIZER,
            TOP_SIZER,
            RIGHT_TOP_SIZER,
            LEFT_SIZER,
            RIGHT_SIZER,
            LEFT_BOTTOM_SIZER,
            BOTTOM_SIZER,
            RIGHT_BOTTOM_SIZER
        } ;
    }

    typedef std::pair<PNode, PMesh> MeshNode;

    typedef std::chrono::nanoseconds Nanoseconds;
    typedef std::chrono::microseconds Microseconds;
    typedef std::chrono::milliseconds Milliseconds;
    typedef std::chrono::seconds Seconds;
    typedef std::chrono::steady_clock Clock;
    typedef Clock::time_point TimePoint;

    enum class Intersection
    {
        OUTSIDE, INTERSECTS, INSIDE
    };

    typedef GLushort IndexType;

    enum class AttributesLoc
    {
        POSITION,
        NORMAL,
        TEXTURECOORD0,
        TEXTURECOORD1,
        COLOR,
        TANGENT,
        BONES_ID,
        BONES_WEIGHT,
        MODEL_MATRIX_ROW0,
        MODEL_MATRIX_ROW1,
        MODEL_MATRIX_ROW2,
        NORMAL_MATRIX_COL0,
        NORMAL_MATRIX_COL1,
        NORMAL_MATRIX_COL2,
        MAX_ATTS
    };

    typedef std::vector<IndexType> Indexes;

    struct UniformsUpdate;
    typedef std::set<UniformsUpdate*> UniformObjs;

    enum class CullFaceMode
    {
        DEFAULT,
		BACK = DEFAULT,
        FRONT,
        FRONT_AND_BACK,
        DISABLED,
		MAX_INDEX
    };

    enum class FrontFaceMode
    {
        DEFAULT = GL_CCW,
        CCW = GL_CCW,
        CW = GL_CW
    };

    enum class DrawMode
    {
        SOLID,
        WIREFRAME
    };

    enum class UseBuffer
    {
        NONE,
        DEPTH,
        DEPTH_STENCIL
    };

    enum TransformSpace
    {
        TS_LOCAL,
        TS_PARENT,
        TS_WORLD,
    };

    struct RayNodeResult
    {
        float distance_;
        SceneNode* node_;
    };

    enum class TextureFlag
    {
        NONE = 0,
        GENERATE_MIPMAPS = 1 << 0,
        INVERT_Y = 1 << 1
    };

    typedef FlagSet<TextureFlag> TextureFlags;

    enum class TextureWrapMode
    {
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        REPEAT = GL_REPEAT
    };

    enum class TextureFilterMode
    {
        NEAREST,
        BILINEAR,
        TRILINEAR
    };

    enum class AnimationChannel
    {
        NONE = 0,
        POSITION = 1 << 0,
        ROTATION = 1 << 1,
        SCALE = 1 << 2
    };

    typedef FlagSet<AnimationChannel> AnimationChannelMask;

    enum PhysicsShape
    {
        SH_UNKNOWN,
        SH_BOX,
        SH_CONE,
        SH_CYLINDER,
        SH_CAPSULE,
        SH_SPHERE,
        SH_CONVEX_TRIMESH,
        SH_TRIMESH,
		MAX_INDEX
    };

    enum class CollisionMask
    {
        NONE = 0,
        PARTICLE = 1 << 0,
        ALL = -1
    };

    enum class WindowFlag
    {
        NONE = 0,
        SHOWN = 1 << 0,
        HIDDEN = 1 << 1,
    };

    typedef FlagSet<WindowFlag> WindowFlags;

    struct ContactPoint
    {
        SceneNode* collider_;
        Vector3 normalB_;
        float appliedImpulse_;
        float appliedImpulseLateral1_;
        float appliedImpulseLateral2_;
        float contactMotion1_;
        float contactMotion2_;
        float contactCFM1_;
        float contactCFM2_;
        Vector3 lateralFrictionDir1_;
        Vector3 lateralFrictionDir2_;
    };

    enum class BlendFilterMode
    {
        ADDITIVE,   // strong result, high overexposure
        SCREEN,     //mild result, medium overexposure
        SOFTLIGHT   //light result, no overexposure
    };

    struct BlurFilter
    {
        Vector2 blurDir_;
        /// This should usually be equal to 1.0f / texture_pixel_width for a horizontal blur, and
        /// 1.0f / texture_pixel_height for a vertical blur.
        Vector2 blurRadius_; //calculated in Material::IsValid()
        float sigma_;
        BlurFilter();
        bool operator != (const BlurFilter& uid) const;
    };

    struct WaveFilter
    {
        float factor_;
        float offset_;
        WaveFilter();
        bool operator != (const WaveFilter& uid) const;
    };

    enum class ResourceType
    {
        File,
        Memory
    };

    enum class JoystickAxis
    {
        UNKNOWN = -1,
        FIRST = 0,
        LEFTX = FIRST,
        LEFTY,
        RIGHTX,
        RIGHTY,
        TRIGGERLEFT,
        TRIGGERRIGHT,
        LAST
    };

    class Engine;

    struct SignalSizeChanged : Signal<int, int> {};
    struct SignalFloatFloat : Signal<float, float> {};
    struct SignalMouseButton : Signal<int, float, float> {};
    struct SignalKey : Signal<int, int, int> {};
    struct SignalUnsigned : Signal<unsigned int> {};
    struct SignalMultiGesture : Signal<int, float, float, float, float, int> {};
    struct SignalUpdate : Signal<float> {};
    struct SignalCollision : Signal<const ContactPoint&> {};
    struct SignalDropFile : Signal<const std::string&> {};
    struct SignalNodeMouseMoved : Signal<SceneNode*, float, float> {};
    struct SignalNodeMouseButton : Signal<SceneNode*, int, float, float> {};
    struct SignalNodeKey : Signal<SceneNode*, int, int, int> {};
    struct SignalNodeChar : Signal<SceneNode*, unsigned int> {};
    struct SignalButtonMouse : Signal<int> {};
    struct SignalEmpty : Signal<> {};
    struct SignalWindow : Signal<Window*> {};
    struct SignalJoystickButton : Signal<int, unsigned> {};
    struct SignalJoystickAxisMotion : Signal<int, JoystickAxis, float> {};
    struct SignalHTTPRequestOnLoad : Signal<std::string> {};
    struct SignalHTTPRequestOnError : Signal<int, std::string> {};
    struct SignalEngine : Signal<Engine*> {};
    struct SignalLight : Signal<Light*> {};
    struct SignalCamera : Signal<Camera*> {};
    struct SignalParticleSystem : Signal<ParticleSystem*> {};

    enum MaterialTexture
    {
        DIFFUSE_MAP,
        NORMAL_MAP,
        LIGHT_MAP,
        SPECULAR_MAP,
        AO_MAP,
        MAX_MATERIAL_MAPS,
        SHADOW_MAP = MAX_MATERIAL_MAPS,
        MAX_MAPS
    };

    enum ParticleSystemEmitFrom
    {
        PS_EF_VERTS,
        PS_EF_ORIGIN
    };

    enum ParticleSystemDistribution
    {
        LINEARY,
        RANDOM
    };

    enum class SceneNodeFlag
    {
        NONE = 0,
        ALLOW_RAY_QUERY = 1 << 0,
    };

    typedef FlagSet<SceneNodeFlag> SceneNodeFlags;

    enum class DepthFunc
    {
        NEVER,
        LESS,
        EQUAL,
        LEQUAL,
        GREATER,
        NOTEQUAL,
        GEQUAL,
        ALWAYS
    };

    enum class CameraSensorFit //the same as in Blender
    {
        AUTOMATIC,
        HORIZONTAL,
        VERTICAL
    };

    struct OrthoProjection
    {
        float left_;
        float right_;
        float bottom_;
        float top_;
        float near_;
        float far_;
    };

    enum class HTTPRequestStatus
    {
        NONE,
        SUCCESSFUL,
        FAILED,
        IN_PROGRESS
    };

    enum class TextureFormat
    {
        UNKNOWN,
        ALPHA,
        RGB,
        RGBA,
        DEPTH,
        DXT1,
        DXT3,
        DXT5,
        ETC1,
        PVRTC_RGB_2BPP,
        PVRTC_RGBA_2BPP,
        PVRTC_RGB_4BPP,
        PVRTC_RGBA_4BPP
    };

    enum class FillMode
    {
        SOLID,
        WIREFRAME,
        POINT,
		MAX_INDEX
    };

    enum class LightingMode
    {
        VERTEXCOLOR,
        UNLIT,
        PERVERTEX,
        PERPIXEL
    };

    enum class PassType
    {
        AMBIENT,
        SHADOW,
        LIT
    };

    enum class RenderPass
    {
        VERTEXCOLOR,
        UNLIT,
        PERVERTEX,
        PERPIXEL,
        TEXT,
        BLEND,
        BLUR,
        WAVE,
        SHOW_TEXTURE0,
        SHOW_DEPTH,
		MAX_INDEX
    };

    enum class BillboardType
    {
        NONE,
        SPHERICAL,
        CYLINDRICAL
    };

    enum class CubeMapFace
    {
        FACE_POSITIVE_X = 0,
        FACE_NEGATIVE_X,
        FACE_POSITIVE_Y,
        FACE_NEGATIVE_Y,
        FACE_POSITIVE_Z,
        FACE_NEGATIVE_Z,
        MAX_CUBEMAP_FACES
    };

    enum class TextureTarget
    {
        UNKNOWN = -1,
        TEXTURE_2D = GL_TEXTURE_2D,
        TEXTURE_CUBE_MAP_POSITIVE_X = GL_TEXTURE_CUBE_MAP_POSITIVE_X, 
        TEXTURE_CUBE_MAP_NEGATIVE_X = GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 
        TEXTURE_CUBE_MAP_POSITIVE_Y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
        TEXTURE_CUBE_MAP_NEGATIVE_Y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 
        TEXTURE_CUBE_MAP_POSITIVE_Z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 
        TEXTURE_CUBE_MAP_NEGATIVE_Z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };
}
