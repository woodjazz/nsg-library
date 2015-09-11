#pragma once
namespace NSG
{
static const char* VS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(COMPILEVS) && !defined(HAS_USER_VERTEX_SHADER)\n"\
"	// use always gl_Position = GetClipPos() between different passes\n"\
"	// to avoid variation and z-fighting issues.\n"\
"	// For example do not use:\n"\
"	//		gl_Position = GetClipPos(); for AMBIENT pass\n"\
"	//		gl_Position = GetClipPos(worldPos); for PER_PIXEL_LIGHTING pass\n"\
"	// since it can produce different results and cause z-fighting between passes\n"\
"	void main()\n"\
"	{\n"\
"		#if defined(VERTEXCOLOR)\n"\
"			\n"\
"			v_color = a_color;\n"\
"			gl_Position = GetClipPos();\n"\
"		#elif defined(TEXT)\n"\
"			gl_Position = GetClipPos();\n"\
"			v_texcoord0 = GetTexCoord(a_texcoord0, u_uvTransform0);\n"\
"		#elif defined(BLUR) || defined(BLEND) || defined(WAVE) || defined(SHOW_TEXTURE0)\n"\
"			gl_Position = vec4(a_position, 1.0);\n"\
"			v_texcoord0 = GetTexCoord(a_texcoord0, u_uvTransform0);\n"\
"		#elif defined(AMBIENT) || defined(UNLIT)\n"\
"		\n"\
"			gl_Position = GetClipPos();\n"\
"			v_texcoord0 = GetTexCoord(a_texcoord0, u_uvTransform0);\n"\
"			v_texcoord1 = GetTexCoord(a_texcoord1, u_uvTransform1);\n"\
"			#ifdef FOG\n"\
"				v_worldPos = GetWorldPos().xyz;\n"\
"				v_depth = -GetCameraPos().z;\n"\
"			#endif\n"\
"		#elif defined(SHADOWCUBE_PASS) || defined(SHADOW_PASS) || defined(SHADOW_PASS_SPOT)\n"\
"			gl_Position = GetClipPos();\n"\
"			v_worldPos = GetWorldPos().xyz;\n"\
"		#elif defined(PER_VERTEX_LIGHTING)\n"\
"			v_worldPos = GetWorldPos().xyz;\n"\
"		    vec3 normal = GetWorldNormal();\n"\
"		    vec3 vertexToEye = normalize(u_eyeWorldPos - v_worldPos);\n"\
"		    vec3 world2light = worldPos - u_lightPosition[0];\n"\
"		    vec4 totalLight = vec4(1.0);//CalcTotalLight(world2light, vertexToEye, normal);\n"\
"		    v_color = totalLight;\n"\
"			v_texcoord0 = GetTexCoord(a_texcoord0, u_uvTransform0);\n"\
"			gl_Position = GetClipPos();\n"\
"		#elif defined(PER_PIXEL_LIGHTING)\n"\
"			//Lighting is calculated in world space\n"\
"			v_worldPos = GetWorldPos().xyz;\n"\
"			v_vertexToEye = normalize(u_eyeWorldPos - v_worldPos);\n"\
"			v_normal = GetWorldNormal();\n"\
"			#ifdef FOG\n"\
"				v_depth = -GetCameraPos().z;\n"\
"			#endif\n"\
"			\n"\
"			#if defined(NORMALMAP0) || defined(NORMALMAP1)\n"\
"				v_tangent = GetWorldTangent(); // Transform the tangent vector to world space (and pass it to the fragment shader).\n"\
"			    v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);\n"\
"			    v_bitangent = cross(v_tangent, v_normal);\n"\
"			    // v_normal, v_tangent and v_bitangent are in world coordinates\n"\
"			#endif\n"\
"			v_texcoord0 = GetTexCoord(a_texcoord0, u_uvTransform0);\n"\
"			v_texcoord1 = GetTexCoord(a_texcoord1, u_uvTransform1);\n"\
"			gl_Position = GetClipPos();\n"\
"		#elif defined(IMGUI)\n"\
"			v_color = a_color;\n"\
"			v_texcoord0 = a_texcoord0;\n"\
"			gl_Position = u_viewProjection * vec4(a_position.xy, 0.0, 1.0);\n"\
"		#endif\n"\
"	}\n"\
"#endif\n"\
;
}
