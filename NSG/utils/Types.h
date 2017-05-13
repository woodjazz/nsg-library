/*
-------------------------------------------------------------------------------
This file is part of nsg-library.
http://github.com/woodjazz/nsg-library

Copyright (c) 2014-2017 Néstor Silveira Gorski

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
#include "FlagSet.h"
#include "SharedPointers.h"
#include "SignalSlots.h"
#include <chrono>
#include <string>

namespace pugi {
class xml_node;
class xml_document;
}

namespace NSG {
class Path;

struct GLException : std::exception {
    int code;
    GLException(int e) : code(e) {}
};

enum class LightType { POINT, DIRECTIONAL, SPOT, MAX_INDEX };

enum class BLEND_MODE { NONE, ALPHA, MULTIPLICATIVE, ADDITIVE, MAX_INDEX };

enum class TextureBlend {
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

enum class TextureType {
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

typedef enum {
    LEFT_ALIGNMENT,
    CENTER_ALIGNMENT,
    RIGHT_ALIGNMENT
} HorizontalAlignment;
typedef enum {
    TOP_ALIGNMENT,
    MIDDLE_ALIGNMENT,
    BOTTOM_ALIGNMENT
} VerticalAlignment;

typedef std::chrono::milliseconds Milliseconds;
typedef std::chrono::seconds Seconds;
typedef std::chrono::steady_clock Clock;
typedef Clock::time_point TimePoint;

enum class Intersection { OUTSIDE, INTERSECTS, INSIDE };

typedef unsigned short IndexType;

enum class AttributesLoc {
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

enum class CullFaceMode {
    DEFAULT,
    BACK = DEFAULT,
    FRONT,
    FRONT_AND_BACK,
    DISABLED,
    MAX_INDEX
};

enum class DrawMode { SOLID, WIREFRAME };

enum class UseBuffer { NONE, DEPTH, DEPTH_STENCIL };

enum TransformSpace {
    TS_LOCAL,
    TS_PARENT,
    TS_WORLD,
};

struct RayNodeResult {
    float distance_;
    SceneNode* node_;
    RayNodeResult();
    RayNodeResult(float distance, SceneNode* node);
};

enum class TextureFlag {
    NONE = 0,
    GENERATE_MIPMAPS = 1 << 0,
    INVERT_Y = 1 << 1
};

typedef FlagSet<TextureFlag> TextureFlags;

enum class TextureWrapMode {
    CLAMP_TO_EDGE,
    MIRRORED_REPEAT,
    REPEAT,
    MAX_INDEX
};

enum class TextureFilterMode { NEAREST, BILINEAR, TRILINEAR, MAX_INDEX };

enum class AnimationChannel {
    NONE = 0,
    POSITION = 1 << 0,
    ROTATION = 1 << 1,
    SCALE = 1 << 2,
    ALL = POSITION | ROTATION | SCALE
};

typedef FlagSet<AnimationChannel> AnimationChannelMask;

enum PhysicsShape {
    SH_EMPTY,
    SH_BOX,
    SH_CONE_Z,
    SH_CONE_Y,
    SH_CONE_X,
    SH_CYLINDER_Z,
    SH_CYLINDER_Y,
    SH_CYLINDER_X,
    SH_CAPSULE_Z,
    SH_CAPSULE_Y,
    SH_CAPSULE_X,
    SH_SPHERE,
    SH_CONVEX_TRIMESH,
    SH_TRIMESH,
    MAX_INDEX
};

enum PhysicsBody {
    BODY_NO_COLLISION,
    BODY_STATIC,
    BODY_DYNAMIC,
    BODY_RIGID,
    BODY_SOFT,
    BODY_OCCLUDER,
    BODY_SENSOR,
    BODY_NAVMESH,
    BODY_CHARACTER,
    BODY_UNKNOWN
};

enum class CollisionMask {
    NONE = 0,
    PARTICLE = 1 << 0,
    PLAYER = 1 << 1,
    ENEMY = 1 << 2,
    ALL = -1
};

enum class WindowFlag {
    NONE = 0,
    SHOWN = 1 << 0,
    HIDDEN = 1 << 1,
};

typedef FlagSet<WindowFlag> WindowFlags;

enum class BlendFilterMode {
    ADDITIVE, // strong result, high overexposure
    SCREEN,   // mild result, medium overexposure
    SOFTLIGHT // light result, no overexposure
};

enum class JoystickAxis {
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

enum class JoystickButton {
    UNKNOWN = -1,
    FIRST = 0,
    BUTTON_A = FIRST,
    BUTTON_B,
    BUTTON_X,
    BUTTON_Y,
    BUTTON_BACK,
    BUTTON_GUIDE,
    BUTTON_START,
    BUTTON_LEFTSTICK,
    BUTTON_RIGHTSTICK,
    BUTTON_LEFTSHOULDER,
    BUTTON_RIGHTSHOULDER,
    BUTTON_DPAD_UP,
    BUTTON_DPAD_DOWN,
    BUTTON_DPAD_LEFT,
    BUTTON_DPAD_RIGHT,
    LAST
};

class Engine;

struct TouchFingerEvent {
    enum class Type { MOTION, DOWN, UP };
    Type type;
    int64_t touchId;
    int64_t fingerId;
    float x;
    float y;
    float dx;
    float dy;
    float pressure;
};

struct SignalSizeChanged : Signal<int, int> {};
struct SignalFloatFloat : Signal<float, float> {};
struct SignalMouseButton : Signal<int, float, float> {};
struct SignalMouseButtonInt : Signal<int, int, int> {};
struct SignalMouseMoved : Signal<int, int> {};
struct SignalKey : Signal<int, int, int> {};
struct SignalUnsigned : Signal<unsigned int> {};
struct SignalText : Signal<std::string> {};
struct SignalMultiGesture : Signal<int, float, float, float, float, int> {};
struct SignalUpdate : Signal<float> {};
struct SignalDropFile : Signal<const std::string&> {};
struct SignalNodeMouseMoved : Signal<SceneNode*, float, float> {};
struct SignalNodeMouseButton : Signal<SceneNode*, int, float, float> {};
struct SignalNodeKey : Signal<SceneNode*, int, int, int> {};
struct SignalNodeChar : Signal<SceneNode*, unsigned int> {};
struct SignalEmpty : Signal<> {};
struct SignalBool : Signal<bool> {};
struct SignalFloat : Signal<float> {};
struct SignalWindow : Signal<PWindow> {};
struct SignalJoystickButton : Signal<int, JoystickButton> {};
struct SignalJoystickAxisMotion : Signal<int, JoystickAxis, float> {};
struct SignalHTTPRequestOnLoad : Signal<std::string> {};
struct SignalHTTPRequestOnError : Signal<int, std::string> {};
struct SignalEngine : Signal<Engine*> {};
struct SignalLight : Signal<Light*> {};
struct SignalCamera : Signal<Camera*> {};
struct SignalParticleSystem : Signal<ParticleSystem*> {};
struct SignalDebugRenderer : Signal<DebugRenderer*> {};
struct SignalTouchFinger : Signal<const TouchFingerEvent&> {};

enum MaterialTexture {
    DIFFUSE_MAP,
    NORMAL_MAP,
    AO_MAP = NORMAL_MAP,
    LIGHT_MAP,
    SPECULAR_MAP = LIGHT_MAP,
    SHADOW_MAP0,
    SHADOW_MAP1,
    SHADOW_MAP2,
    SHADOW_MAP3,
    MAX_MAPS
};

enum ParticleSystemEmitFrom { PS_EF_VERTS, PS_EF_ORIGIN };

enum ParticleSystemDistribution { LINEARY, RANDOM };

enum class SceneNodeFlag {
    NONE = 0,
    ALLOW_RAY_QUERY = 1 << 0,
};

typedef FlagSet<SceneNodeFlag> SceneNodeFlags;

enum class DepthFunc {
    NEVER,
    LESS,
    EQUAL,
    LEQUAL,
    GREATER,
    NOTEQUAL,
    GEQUAL,
    ALWAYS
};

enum class TextureFormat {
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

enum class FillMode { SOLID, WIREFRAME, POINT, MAX_INDEX };

enum class PassType { DEFAULT, SHADOW, LIT };

enum class RenderPass {
    VERTEXCOLOR,
    UNLIT,
    LIT,
    TEXT,
    BLEND,
    BLUR,
    WAVE,
    SHOCKWAVE,
    SHOW_TEXTURE0,
    MAX_INDEX
};

enum class BillboardType { NONE, SPHERICAL, CYLINDRICAL, CYLINDRICAL_Z };

enum class CubeMapFace {
    FACE_POSITIVE_X = 0,
    FACE_NEGATIVE_X,
    FACE_POSITIVE_Y,
    FACE_NEGATIVE_Y,
    FACE_POSITIVE_Z,
    FACE_NEGATIVE_Z,
    MAX_CUBEMAP_FACES
};

enum class PixelFormat {
    RGB888,
    RGB565,
    RGBA8888,
    ARGB8888,
    ABGR8888,
    UNKNOWN
};

enum class RendererContext { DEFAULT, EDITOR };
}
