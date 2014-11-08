#pragma once
#include <string>
namespace NSG
{
static const std::string COMMON_GLSL = \
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
"varying vec3 v_normal;\n"\
"varying vec3 v_tangent;\n"\
"varying vec3 v_bitangent;\n"\
"varying vec2 v_texcoord0; \n"\
"varying vec2 v_texcoord1;\n"\
"varying vec3 v_vertexToEye;\n"\
"varying vec3 v_vertexToEyeInTangentSpace;\n"\
"varying vec3 v_lightDirection[NUM_POINT_LIGHTS_ARRAY];\n"\
"varying vec3 v_light2Pixel[NUM_SPOT_LIGHTS_ARRAY];\n"\
"#if defined(COMPILEFS)\n"\
"	\n"\
"	uniform sampler2D u_texture0;\n"\
"	uniform sampler2D u_texture1;\n"\
"	uniform sampler2D u_texture2;\n"\
"	uniform sampler2D u_texture3;\n"\
"	uniform sampler2D u_texture4;\n"\
"	uniform sampler2D u_texture5;\n"\
"#endif\n"\
;
}
