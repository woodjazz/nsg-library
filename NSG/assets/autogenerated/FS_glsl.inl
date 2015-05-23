#pragma once
namespace NSG
{
static const char* FS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(COMPILEFS) && !defined(HAS_USER_FRAGMENT_SHADER)\n"\
"	void main()\n"\
"	{\n"\
"		#if defined(AMBIENT)\n"\
"			\n"\
"			gl_FragColor = v_color * GetAmbientLight();\n"\
"		#elif defined(SHADOW)\n"\
"			gl_FragColor = vec4(EncodeDepth2Color(v_color.z), 1.0);\n"\
"		#elif defined(SHADOWCUBE)\n"\
"			vec3 lightToVertex = v_worldPos - u_eyeWorldPos;\n"\
"    		float lightToPixelDistance = clamp(length(lightToVertex) * u_pointLight.invRange, 0.0, 1.0);\n"\
"    		gl_FragColor = vec4(EncodeDepth2Color(lightToPixelDistance), 1.0);\n"\
"		#elif defined(TEXT)\n"\
"			gl_FragColor = v_color * vec4(1.0, 1.0, 1.0, texture2D(u_texture0, v_texcoord0).a);\n"\
"		#elif defined(BLEND)\n"\
"			gl_FragColor = Blend();\n"\
"		#elif defined(BLUR)\n"\
"			gl_FragColor = Blur();\n"\
"		#elif defined(WAVE)\n"\
"			gl_FragColor = Wave();\n"\
"		#elif defined(SHOW_TEXTURE0)\n"\
"			gl_FragColor = texture2D(u_texture0, v_texcoord0);\n"\
"		#elif defined(UNLIT)\n"\
"			#ifdef DIFFUSEMAP\n"\
"				gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);\n"\
"			#else\n"\
"				gl_FragColor = v_color;\n"\
"			#endif\n"\
"		#elif defined(PER_VERTEX_LIGHTING)\n"\
"			#ifdef DIFFUSEMAP\n"\
"				gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);\n"\
"			#else\n"\
"				gl_FragColor = v_color;\n"\
"			#endif\n"\
"		#elif defined(PER_PIXEL_LIGHTING)\n"\
"			//Lighting is calculated in world space\n"\
"			#ifdef NORMALMAP\n"\
"				//The normals in the map are stored in tangent/texture space.\n"\
"				//For better explanation see http://ogldev.atspace.co.uk/www/tutorial26/tutorial26.html\n"\
"				//We need to transform them to world space:\n"\
"				mat3 TBN = mat3(v_tangent, v_bitangent, v_normal); //Generate a world space transformation matrix using the tangent-bitangent-normal.\n"\
"				vec3 bumpMapNormal = texture2D(u_texture1, v_texcoord0).xyz; // Sample the normal from the normal map \n"\
"			    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0); //Normalize from 0..1 to -1..1\n"\
"			    vec3 normal = normalize(TBN * bumpMapNormal); //Transform the normal from tangent/texture space to world space using the TBN matrix\n"\
"			#else\n"\
"				vec3 normal = normalize(v_normal);\n"\
"			#endif\n"\
"    		vec3 vertexToEye = normalize(v_vertexToEye);\n"\
"    		vec4 totalLight = CalcTotalLight(vertexToEye, normal);\n"\
"			#ifdef DIFFUSEMAP\n"\
"	    		gl_FragColor = v_color * totalLight * texture2D(u_texture0, v_texcoord0);\n"\
"	    	#else\n"\
"	    		gl_FragColor = v_color * totalLight;\n"\
"	    	#endif\n"\
"		#else // Vertex color by default\n"\
"			gl_FragColor = v_color;\n"\
"		#endif	    \n"\
"	}	\n"\
"#endif\n"\
;
}
