#include "gles2VertexCompatibility.h"
"\
uniform mat4 u_mvp;\
attribute vec2 a_texcoord;\
attribute vec3 a_color;\
attribute vec4 a_position;\
varying vec3 v_color;\
varying vec2 v_texcoord;\
\
void main() \
{\
	gl_Position = u_mvp * a_position;\
	v_color = a_color;\
	v_texcoord = a_texcoord;\
}\
"
