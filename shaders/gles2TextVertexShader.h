#include "gles2VertexCompatibility.h"
"\
attribute vec4 a_position;\
varying vec2 v_texcoord;\
\
void main() \
{\
	gl_Position = vec4(a_position.xy, 0, 1);\
	v_texcoord = a_position.zw;\
}\
"
