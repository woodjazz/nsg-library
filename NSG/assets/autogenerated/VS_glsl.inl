#pragma once
#include <string>
namespace NSG
{
static const std::string VS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(COMPILEVS) && !defined(HAS_USER_VERTEX_SHADER)\n"\
"	void main()\n"\
"	{\n"\
"		#ifdef SHOW_TEXTURE\n"\
"			gl_Position = vec4(a_position, 1.0);\n"\
"			v_texcoord = vec2(a_texcoord.x, 1.0 - a_texcoord.y);\n"\
"		#elif defined(BLUR) || defined(BLEND)\n"\
"			gl_Position = vec4(a_position, 1.0);\n"\
"			v_texcoord = a_texcoord;\n"\
"		#elif defined(STENCIL)\n"\
"			gl_Position = GetClipPos(GetWorldPos());	\n"\
"		#else\n"\
"			vec3 worldPos = GetWorldPos();\n"\
"			#if defined(PER_VERTEX_LIGHTING)\n"\
"			    vec3 normal = GetWorldNormal();\n"\
"			    vec3 vertexToEye = normalize(u_eyeWorldPos - worldPos);\n"\
"			    vec4 totalLight = CalcVSTotalLight(worldPos, vertexToEye, normal);\n"\
"			    v_color = a_color * totalLight;\n"\
"			#elif defined(PER_PIXEL_LIGHTING)\n"\
"				v_normal = GetWorldNormal();\n"\
"				\n"\
"				#ifdef NORMAL_MAP					\n"\
"					v_tangent = GetWorldTangent();\n"\
"				    v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);\n"\
"				    v_bitangent = cross(v_tangent, v_normal);\n"\
"				#endif\n"\
"				v_color = a_color;				\n"\
"				v_vertexToEye = normalize(u_eyeWorldPos - worldPos);\n"\
"				for (int i = 0 ; i < NUM_POINT_LIGHTS ; i++) \n"\
"					v_lightDirection[i] = worldPos - u_pointLights[i].position;\n"\
"				for (int i = 0 ; i < NUM_SPOT_LIGHTS ; i++) \n"\
"					v_light2Pixel[i] = worldPos - u_spotLights[i].point.position;\n"\
"			#else\n"\
"				v_color = u_material.color * a_color;\n"\
"			#endif\n"\
"		    v_texcoord = a_texcoord;\n"\
"			gl_Position = GetClipPos(worldPos);\n"\
"		#endif\n"\
"	}\n"\
"#endif\n"\
;
}
