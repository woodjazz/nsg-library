#include "gles2VertexCompatibility.h"
"\
uniform mat4 u_mvp;\
attribute vec4 a_position;\
\
void main() \
{\
	gl_Position = u_mvp * vec4(a_position.xyz, 1);\
}\
"
