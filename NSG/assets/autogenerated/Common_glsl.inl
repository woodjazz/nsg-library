#pragma once
namespace NSG
{
static const char* COMMON_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#ifdef GLES2\n"\
"	#ifdef GL_FRAGMENT_PRECISION_HIGH\n"\
"		precision highp float;\n"\
"		precision highp int;\n"\
"	#else\n"\
"		precision mediump float;\n"\
"		precision mediump int;\n"\
"	#endif\n"\
"#else\n"\
"	#define lowp\n"\
"	#define mediump\n"\
"	#define highp\n"\
"#endif   \n"\
"varying vec4 v_color;\n"\
"varying vec2 v_texcoord0;\n"\
"varying vec2 v_texcoord1;\n"\
"varying vec3 v_worldPos;\n"\
"varying vec3 v_normal;\n"\
"varying vec3 v_tangent;\n"\
"varying vec3 v_bitangent;\n"\
"varying vec3 v_vertexToEye;\n"\
"varying float v_depth;\n"\
"struct Material\n"\
"{\n"\
"	vec4 diffuseColor;\n"\
"	float diffuseIntensity;\n"\
"	vec4 specularColor;\n"\
"	float specularIntensity;\n"\
"    float ambientIntensity;\n"\
"    float shininess;\n"\
"    float emitIntensity;\n"\
"};\n"\
"uniform vec3 u_sceneAmbientColor;\n"\
"uniform vec3 u_sceneHorizonColor;\n"\
"uniform Material u_material;\n"\
"uniform vec3 u_eyeWorldPos; // camera or shadowCamera world position\n"\
"// The Z far value in the shadow camera used to know the correct split\n"\
"// one coordinate per split (in case of directional light)\n"\
"uniform vec4 u_shadowCameraZFar;\n"\
"uniform vec4 u_shadowColor;\n"\
"//uniform float u_shadowBias; // constant bias\n"\
"uniform vec4 u_shadowMapInvSize;\n"\
"uniform vec3 u_lightPosition;\n"\
"uniform float u_lightInvRange; // only used for point and spot lights\n"\
"uniform vec3 u_lightDirection;\n"\
"uniform vec4 u_lightDiffuseColor;\n"\
"uniform vec4 u_lightSpecularColor;\n"\
"uniform float u_lightCutOff; // 0.5f * cosine(cutOff)\n"\
"uniform mat4 u_view; // could be main camera or shadowCamera\n"\
"uniform float u_fogMinIntensity;\n"\
"uniform float u_fogStart;\n"\
"uniform float u_fogEnd;\n"\
"uniform float u_fogHeight;\n"\
"#ifdef COMPILEVS\n"\
"	uniform mat4 u_model;\n"\
"	uniform mat3 u_normalMatrix;\n"\
"	uniform mat4 u_viewProjection; // could be main camera or shadowCamera\n"\
"	uniform mat4 u_projection; // could be main camera or shadowCamera\n"\
"	uniform vec4 u_uvTransform0; /* DIFFUSEMAP */\n"\
"	uniform vec4 u_uvTransform1; /* NORMALMAP, AOMAP */	\n"\
"	uniform vec4 u_uvTransform2; /* LIGHTMAP, SPECULARMAP */\n"\
"	#if defined(SKINNED)\n"\
"		uniform mat4 u_bones[MAX_BONES];\n"\
"	#endif\n"\
"#else //COMPILEFS\n"\
"	\n"\
"	#if defined(SPLITS1)\n"\
"		#define MAX_SPLITS 1\n"\
"	#elif defined(SPLITS2)\n"\
"		#define MAX_SPLITS 2\n"\
"	#elif defined(SPLITS3)\n"\
"		#define MAX_SPLITS 3\n"\
"	#elif defined(SPLITS4)\n"\
"		#define MAX_SPLITS 5\n"\
"	#endif\n"\
"	#ifdef MAX_SPLITS		\n"\
"		uniform mat4 u_lightView[MAX_SPLITS];\n"\
"		uniform mat4 u_lightProjection[MAX_SPLITS];\n"\
"		uniform mat4 u_lightViewProjection[MAX_SPLITS];\n"\
"	#endif\n"\
"#endif\n"\
;
}
