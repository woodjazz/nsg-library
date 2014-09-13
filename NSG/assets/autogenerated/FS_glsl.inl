#pragma once
#include <string>
namespace NSG
{
static const std::string FS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(COMPILEFS) && !defined(HAS_USER_FRAGMENT_SHADER)\n"\
"	\n"\
"	void main()\n"\
"	{\n"\
"		#if defined(TEXT)\n"\
"			gl_FragColor = v_color * vec4(1.0, 1.0, 1.0, texture2D(u_texture0, v_texcoord).a);\n"\
"		#elif defined(BLEND)\n"\
"			gl_FragColor = Blend();\n"\
"		#elif defined(BLUR)\n"\
"			gl_FragColor = Blur();\n"\
"		#elif defined(SHOW_TEXTURE)\n"\
"			gl_FragColor = texture2D(u_texture0, v_texcoord);\n"\
"		#elif defined(STENCIL)\n"\
"			gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"\
"		#elif defined(PER_PIXEL_LIGHTING)\n"\
"			vec3 normal = normalize(v_normal);\n"\
"    		vec3 vertexToEye = normalize(v_vertexToEye);\n"\
"    		vec4 totalLight = CalcDirectionalLight(vertexToEye, normal);\n"\
"		    for (int i = 0 ; i < u_numPointLights ; i++) \n"\
"		    {\n"\
"		        totalLight += CalcPointLight(i, vertexToEye, normal);                                            \n"\
"		    }                                                                                       \n"\
"		    gl_FragColor = v_color * u_material.color * totalLight * texture2D(u_texture0, v_texcoord);\n"\
"		#else\n"\
"			gl_FragColor = v_color * texture2D(u_texture0, v_texcoord);\n"\
"		#endif	    \n"\
"	}	\n"\
"#endif\n"\
;
}
