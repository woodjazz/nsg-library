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
"varying vec2 v_texcoord0; \n"\
"#ifdef HAS_LIGHTS\n"\
"	varying vec3 v_normal;\n"\
"	varying vec3 v_tangent;\n"\
"	varying vec3 v_bitangent;\n"\
"	varying vec2 v_texcoord1;\n"\
"	varying vec3 v_vertexToEye;\n"\
"	varying vec3 v_vertexToEyeInTangentSpace;\n"\
"	#ifdef HAS_POINT_LIGHTS\n"\
"		varying vec3 v_lightDirection[NUM_POINT_LIGHTS];\n"\
"	#endif\n"\
"	#ifdef HAS_SPOT_LIGHTS\n"\
"		varying vec3 v_light2Pixel[NUM_SPOT_LIGHTS];\n"\
"	#endif\n"\
"#endif\n"\
;
}
