#include "gles2FragmentCompatibility.h"
"\
struct Material\
{\
	vec4 diffuse;\
};\
uniform Material u_material;\
void main()\
{\
	gl_FragColor = u_material.diffuse;\
}\
"
