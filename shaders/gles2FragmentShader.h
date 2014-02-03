#include "gles2FragmentCompatibility.h"
"\
varying vec3 v_color;\
varying vec2 v_texcoord;\
uniform sampler2D u_texture;\
void main()\
{\
	gl_FragColor = texture2D(u_texture, v_texcoord);\
	gl_FragColor += vec4(v_color, 1);\
}\
"
