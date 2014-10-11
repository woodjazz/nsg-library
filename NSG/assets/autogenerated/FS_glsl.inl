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
"			gl_FragColor = v_color * vec4(1.0, 1.0, 1.0, texture2D(u_texture0, v_texcoord0).a);\n"\
"		#elif defined(BLEND)\n"\
"			gl_FragColor = Blend();\n"\
"		#elif defined(BLUR)\n"\
"			gl_FragColor = Blur();\n"\
"		#elif defined(SHOW_TEXTURE0_INVERT_Y)\n"\
"			gl_FragColor = texture2D(u_texture0, v_texcoord0);\n"\
"		#elif defined(SHOW_TEXTURE0)\n"\
"			gl_FragColor = texture2D(u_texture0, v_texcoord0);\n"\
"		#elif defined(STENCIL)\n"\
"			gl_FragColor = vec4(1.0);\n"\
"		#elif defined(UNLIT)\n"\
"			#ifdef LIGHTMAP\n"\
"				gl_FragColor = v_color * (texture2D(u_texture0, v_texcoord0) + texture2D(u_texture2, v_texcoord1));\n"\
"			#else\n"\
"				gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);\n"\
"			#endif\n"\
"		#elif defined(PER_PIXEL_LIGHTING)\n"\
"			#ifdef NORMALMAP\n"\
"				vec3 bumpMapNormal = texture2D(u_texture1, v_texcoord0).xyz;\n"\
"			    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0);\n"\
"			    mat3 TBN = mat3(v_tangent, v_bitangent, v_normal);\n"\
"			    vec3 normal = normalize(TBN * bumpMapNormal);\n"\
"			#else\n"\
"				vec3 normal = normalize(v_normal);\n"\
"			#endif\n"\
"    		vec3 vertexToEye = normalize(v_vertexToEye);\n"\
"    		vec4 totalLight = CalcFSTotalLight(vertexToEye, normal);\n"\
"    		#ifdef LIGHTMAP\n"\
"    			//gl_FragColor = v_color * totalLight * (0.5 * texture2D(u_texture0, v_texcoord0) + 0.5 * texture2D(u_texture2, v_texcoord1));\n"\
"    			gl_FragColor = texture2D(u_texture2, v_texcoord1);\n"\
"    		#else\n"\
"		    	gl_FragColor = v_color * totalLight * texture2D(u_texture0, v_texcoord0);\n"\
"		    #endif\n"\
"		#else\n"\
"			#ifdef LIGHTMAP\n"\
"				gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0) * texture2D(u_texture2, v_texcoord1);\n"\
"			#else\n"\
"				gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);\n"\
"			#endif\n"\
"		#endif	    \n"\
"	}	\n"\
"#endif\n"\
;
}
