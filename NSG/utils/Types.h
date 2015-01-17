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

    typedef enum {BLEND_NONE, BLEND_ALPHA} BLEND_MODE ;

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

    struct CachedData
    {
        std::vector<PModelMesh> meshes_;
        std::vector<PMaterial> materials_;
    };

    struct RayNodeResult
    {
        float distance_;
        SceneNode* node_;
    };

    enum class ProgramFlag
    {
        NONE = 0,
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
        DIFFUSEMAP = 1 << 14
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
        SH_SPHERE,
        SH_CONVEX_TRIMESH,
        SH_TRIMESH
    };

    struct ContactPoint
    {
        SceneNode* collider_;
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
        Vector2 blurRadius_;
        float sigma_;
        BlurFilter();
        bool operator != (const BlurFilter& uid) const;
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
    typedef Signal<> SignalResourceLoaded;
    typedef Signal<const ContactPoint&> SignalCollision;
    typedef Signal<const std::string&> SignalDropFile;


}
