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
"varying vec2 v_texcoord; \n"\
"varying vec3 v_vertexToEye;\n"\
"varying vec3 v_lightDirection[MAX_POINT_LIGHTS];\n"\
"#if defined(COMPILEFS)\n"\
"	\n"\
"	uniform sampler2D u_texture0;\n"\
"	uniform sampler2D u_texture1;\n"\
"	\n"\
"#endif\n"\
;
}
