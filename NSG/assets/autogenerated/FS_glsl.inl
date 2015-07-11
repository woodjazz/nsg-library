#pragma once
namespace NSG
{
static const char* FS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(COMPILEFS) && !defined(HAS_USER_FRAGMENT_SHADER)\n"\
"	void main()\n"\
"	{\n"\
"		#if defined(VERTEXCOLOR)\n"\
"			gl_FragColor = v_color;\n"\
"		#elif defined(TEXT)\n"\
"			gl_FragColor = vec4(vec3(u_material.diffuseColor), GetDiffuseColor().a);\n"\
"		#elif defined(BLEND)\n"\
"			gl_FragColor = Blend();\n"\
"		#elif defined(BLUR)\n"\
"			gl_FragColor = Blur();\n"\
"		#elif defined(WAVE)\n"\
"			gl_FragColor = Wave();\n"\
"		#elif defined(SHOW_TEXTURE0)\n"\
"			gl_FragColor = texture2D(u_texture0, v_texcoord0);\n"\
"		#elif defined(AMBIENT) || defined(UNLIT)\n"\
"			\n"\
"			#ifdef FOG\n"\
"				vec4 finalColor = GetAmbientIntensity() * GetDiffuseColor();\n"\
"				gl_FragColor = vec4(GetAmbientFog(finalColor.rgb), finalColor.a);\n"\
"			#else\n"\
"				gl_FragColor = GetAmbientIntensity() * GetDiffuseColor();\n"\
"			#endif\n"\
"		#elif defined(SHADOWCUBE_PASS) || defined(SHADOW_PASS)\n"\
"			vec3 lightToVertex = v_worldPos - u_eyeWorldPos;\n"\
"    		float lightToPixelDistance = length(lightToVertex) * GetLightInvRange();\n"\
"    		gl_FragColor = EncodeDepth2Color(lightToPixelDistance);\n"\
"    	#elif defined(PER_VERTEX_LIGHTING)\n"\
"			gl_FragColor = v_color * GetDiffuseColor();\n"\
"		#elif defined(PER_PIXEL_LIGHTING)\n"\
"				//Lighting is calculated in world space\n"\
"				vec3 normal = GetNormal();\n"\
"	    		vec3 vertexToEye = normalize(v_vertexToEye);\n"\
"	    		vec3 world2light = v_worldPos - GetLightPosition();\n"\
"				#ifdef FOG\n"\
"					vec4 finalColor = CalcTotalLight(world2light, vertexToEye, normal) * GetDiffuseColor();\n"\
"					gl_FragColor = vec4(GetLitFog(finalColor.rgb), finalColor.a);\n"\
"				#else\n"\
"			    	gl_FragColor = CalcTotalLight(world2light, vertexToEye, normal) * GetDiffuseColor();\n"\
"			    #endif\n"\
"		#endif	    \n"\
"	}	\n"\
"#endif\n"\
;
}
