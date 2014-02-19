#include "gles2VertexCompatibility.h"
"\
attribute vec4 a_position;\
\
void main() \
{\
	gl_Position = u_mvp * a_position;\
}\
"
