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

    enum class LightType {POINT, DIRECTIONAL, SPOT};

    typedef enum
    {
        BLEND_NONE,
        BLEND_ALPHA,
        BLEND_MULTIPLICATIVE,
        BLEND_ADDITIVE
    } BLEND_MODE ;

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
        DEFAULT = GL_BACK,
        BACK = GL_BACK,
        FRONT = GL_FRONT,
        FRONT_AND_BACK = GL_FRONT_AND_BACK
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

    enum class ProgramFlag
    {
        NONE = 0, // vertex color by default
        PER_VERTEX_LIGHTING = 1 << 0,
        PER_PIXEL_LIGHTING = 1 << 1,
        BLEND = 1 << 2,
        BLUR = 1 << 3,
        TEXT = 1 << 4,
        SHOW_TEXTURE0 = 1 << 5,
        STENCIL = 1 << 6,
        NORMALMAP = 1 << 7,
        LIGHTMAP = 1 << 8,
        UNLIT = 1 << 9,
        SKINNED = 1 << 10,
        SPECULARMAP = 1 << 11,
        AOMAP = 1 << 12,
        DISPLACEMENTMAP = 1 << 13,
        DIFFUSEMAP = 1 << 14,
        SPHERICAL_BILLBOARD = 1 << 15,
        CYLINDRICAL_BILLBOARD = 1 << 16,
        FLIP_Y = 1 << 17,
        WAVE = 1 << 18
    };

    typedef FlagSet<ProgramFlag> ProgramFlags;

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
        SH_TRIMESH
    };

    enum class CollisionMask
    {
        NONE = 0,
        PARTICLE = 1 << 0,
        ALL = -1
    };

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

    typedef Signal<int, int> SignalViewChanged;
    typedef Signal<float, float> SignalMouseMoved;
    typedef Signal<int, float, float> SignalMouseDown;
    typedef Signal<int, float, float> SignalMouseUp;
    typedef Signal<float, float> SignalMouseWheel;
    typedef Signal<int, int, int> SignalKey;
    typedef Signal<unsigned int> SignalChar;
    typedef Signal<int, float, float, float, float, int> SignalMultiGesture;
    typedef Signal<float> SignalUpdate;
    typedef Signal<> SignalRender;
    typedef Signal<const ContactPoint&> SignalCollision;
    typedef Signal<const std::string&> SignalDropFile;
    typedef Signal<SceneNode*, float, float> SignalNodeMouseMoved;
    typedef Signal<SceneNode*, int, float, float> SignalNodeMouseDown;
    typedef Signal<SceneNode*, int, float, float> SignalNodeMouseUp;
    typedef Signal<SceneNode*, float, float> SignalNodeMouseWheel;
    typedef Signal<SceneNode*, int, int, int> SignalNodeKey;
    typedef Signal<SceneNode*, unsigned int> SignalNodeChar;
    typedef Signal<int> SignalButtonMouse;
    typedef Signal<> SignalEmpty;
    typedef Signal<Window*> SignalWindow;
    
    
    enum MaterialTexture
    {
        DIFFUSE_MAP,
        NORMAL_MAP,
        LIGHT_MAP,
        SPECULAR_MAP,
        AO_MAP,
        DISPLACEMENT_MAP,
        MAX_TEXTURES_MAPS
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

    enum class RenderLayer
    {
        DEFAULT_LAYER,
        PARTICLES_LAYER,
        GUI_LAYER0,
        GUI_LAYER1,
        MAX_LAYERS
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
}
