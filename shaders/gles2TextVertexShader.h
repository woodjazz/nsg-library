#include "gles2VertexCompatibility.h"
"\
uniform mat4 u_mvp;\
attribute vec4 a_position;\
varying vec2 v_texcoord;\
\
void main() \
{\
	gl_Position = u_mvp * vec4(a_position.xy, 0, 1);\
	v_texcoord = a_position.zw;\
}\
"
