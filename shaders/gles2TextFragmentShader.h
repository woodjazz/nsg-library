#include "gles2FragmentCompatibility.h"
"\
varying vec2 v_texcoord;\
uniform sampler2D u_texture;\
uniform vec4 u_color;\
void main()\
{\
	gl_FragColor = vec4(1, 1, 1, texture2D(u_texture, v_texcoord).a) * u_color;\
}\
"
