#pragma once
namespace NSG
{
static const char* VS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(COMPILEVS) && !defined(HAS_USER_VERTEX_SHADER)\n"\
"	void main()\n"\
"	{\n"\
"		#if defined(TEXT)\n"\
"			v_color = u_material.color * a_color;\n"\
"			gl_Position = GetClipPos();\n"\
"			v_texcoord0 = GetTexCoord(a_texcoord0);\n"\
"		#elif defined(BLUR) || defined(BLEND) || defined(WAVE)\n"\
"			gl_Position = vec4(a_position, 1.0);\n"\
"			v_texcoord0 = GetTexCoord(a_texcoord0);\n"\
"		#elif defined(SHOW_TEXTURE0)\n"\
"			gl_Position = vec4(a_position, 1.0);\n"\
"			v_texcoord0 = GetTexCoord(a_texcoord0);\n"\
"		#elif defined(UNLIT)\n"\
"			v_color = u_material.color * a_color;\n"\
"			gl_Position = GetClipPos();\n"\
"			v_texcoord0 = GetTexCoord(a_texcoord0);\n"\
"		#elif defined(LIGHTMAP) // lightmap without lighting\n"\
"			v_color = u_material.color * a_color;\n"\
"			v_texcoord0 = GetTexCoord(a_texcoord0);\n"\
"			v_texcoord1 = GetTexCoord(a_texcoord1);\n"\
"			\n"\
"			gl_Position = GetClipPos();\n"\
"		#elif defined(PER_VERTEX_LIGHTING)\n"\
"			vec4 worldPos = GetWorldPos();\n"\
"		    vec3 normal = GetWorldNormal();\n"\
"		    vec3 vertexToEye = normalize(u_eyeWorldPos - worldPos.xyz);\n"\
"		    vec4 totalLight = CalcVSTotalLight(worldPos.xyz, vertexToEye, normal);\n"\
"		    v_color = a_color * totalLight;\n"\
"			v_texcoord0 = GetTexCoord(a_texcoord0);\n"\
"			\n"\
"			#if defined(AOMAP)\n"\
"				v_texcoord1 = GetTexCoord(a_texcoord1);\n"\
"			#endif\n"\
"			gl_Position = GetClipPos(worldPos);\n"\
"		#elif defined(PER_PIXEL_LIGHTING)\n"\
"			//Lighting is calculated in world space\n"\
"			vec4 worldPos = GetWorldPos();\n"\
"			v_vertexToEye = normalize(u_eyeWorldPos - worldPos.xyz);\n"\
"			v_normal = GetWorldNormal();\n"\
"			\n"\
"			#if defined(NORMALMAP)\n"\
"				v_tangent = GetWorldTangent(); // Transform the tangent vector to world space (and pass it to the fragment shader).\n"\
"			    v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);\n"\
"			    v_bitangent = cross(v_tangent, v_normal);\n"\
"			    // v_normal, v_tangent and v_bitangent are in world coordinates\n"\
"			#endif\n"\
"			#ifdef HAS_POINT_LIGHT\n"\
"				v_lightDirection = worldPos.xyz - u_pointLight.position;\n"\
"			#elif defined(HAS_SPOT_LIGHT)\n"\
"				v_light2Pixel = worldPos.xyz - u_spotLight.position;\n"\
"					\n"\
"			#endif\n"\
"			v_color = a_color;\n"\
"			v_texcoord0 = GetTexCoord(a_texcoord0);\n"\
"			#if defined(AOMAP)\n"\
"				v_texcoord1 = GetTexCoord(a_texcoord1);\n"\
"			#endif\n"\
"			gl_Position = GetClipPos(worldPos);\n"\
"		#else // Vertex color by default\n"\
"			v_color = u_material.color * a_color;\n"\
"			gl_Position = GetClipPos();\n"\
"			\n"\
"		#endif\n"\
"	}\n"\
"#endif\n"\
;
}
