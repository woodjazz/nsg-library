#pragma once
#include <string>
namespace NSG
{
static const std::string TRANSFORMS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#ifdef COMPILEVS\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	// Vertex shader specific\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	attribute vec4 a_position;\n"\
"	attribute vec2 a_texcoord;\n"\
"	attribute vec3 a_normal;\n"\
"	attribute vec4 a_color;\n"\
"	#if defined(INSTANCED)\n"\
"	    attribute vec4 a_mMatrixRow0;\n"\
"	    attribute vec4 a_mMatrixRow1;\n"\
"	    attribute vec4 a_mMatrixRow2;\n"\
"	    attribute vec3 a_normalMatrixCol0;\n"\
"	    attribute vec3 a_normalMatrixCol1;\n"\
"	    attribute vec3 a_normalMatrixCol2;\n"\
"	#endif\n"\
"	uniform mat4 u_model;\n"\
"	uniform mat3 u_normalMatrix;\n"\
"	uniform mat4 u_viewProjection; \n"\
"	\n"\
"	mat4 GetModelMatrix()\n"\
"	{\n"\
"		#if defined(INSTANCED)\n"\
"			// Since we are using rows instead of cols the instancing model matrix is a transpose, \n"\
"			// so the matrix multiply order must be swapped\n"\
"			const vec4 lastColumn = vec4(0.0, 0.0, 0.0, 1.0);\n"\
"		    return mat4(a_mMatrixRow0, a_mMatrixRow1, a_mMatrixRow2, lastColumn);\n"\
"	    #else\n"\
"		    return u_model;\n"\
"	    #endif\n"\
"	}\n"\
"	mat3 GetNormalMatrix()\n"\
"	{\n"\
"		#if defined(INSTANCED)\n"\
"			return mat3(a_normalMatrixCol0, a_normalMatrixCol1, a_normalMatrixCol2);\n"\
"		#else\n"\
"			return u_normalMatrix;\n"\
"		#endif\n"\
"	}\n"\
"	vec3 GetWorldPos()\n"\
"	{\n"\
"		#if defined(INSTANCED)\n"\
"			// Instancing model matrix is a transpose, so the matrix multiply order must be swapped\n"\
"			return (a_position * GetModelMatrix()).xyz;\n"\
"		#else\n"\
"	    	return (GetModelMatrix() * a_position).xyz;\n"\
"	    #endif\n"\
"	}\n"\
"	vec3 GetWorldNormal()\n"\
"	{\n"\
"		return normalize((GetNormalMatrix() * a_normal));\n"\
"	}\n"\
"	vec4 GetClipPos(vec3 worldPos)\n"\
"	{\n"\
"	    return u_viewProjection * vec4(worldPos, 1.0);\n"\
"	}\n"\
"#endif\n"\
;
}
