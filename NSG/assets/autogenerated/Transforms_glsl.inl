#pragma once
namespace NSG
{
static const char* TRANSFORMS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#ifdef COMPILEVS\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	// Vertex shader specific\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	attribute vec3 a_position;\n"\
"	attribute vec2 a_texcoord0;\n"\
"	attribute vec2 a_texcoord1;\n"\
"	attribute vec3 a_normal;\n"\
"	attribute vec4 a_color;\n"\
"	attribute vec3 a_tangent;\n"\
"	attribute vec4 a_boneIDs;\n"\
"	attribute vec4 a_weights;\n"\
"	\n"\
"	#if defined(INSTANCED)\n"\
"	    attribute vec4 a_mMatrixRow0;\n"\
"	    attribute vec4 a_mMatrixRow1;\n"\
"	    attribute vec4 a_mMatrixRow2;\n"\
"	    attribute vec3 a_normalMatrixCol0;\n"\
"	    attribute vec3 a_normalMatrixCol1;\n"\
"	    attribute vec3 a_normalMatrixCol2;\n"\
"	#endif\n"\
"	#if defined(SKINNED)\n"\
"		uniform mat4 u_bones[NUM_BONES];\n"\
"		mat4 GetSkinnedMatrix()\n"\
"		{\n"\
"			mat4 boneTransform = u_bones[int(a_boneIDs[0])] * a_weights[0];\n"\
"		    boneTransform += u_bones[int(a_boneIDs[1])] * a_weights[1];\n"\
"		    boneTransform += u_bones[int(a_boneIDs[2])] * a_weights[2];\n"\
"		    boneTransform += u_bones[int(a_boneIDs[3])] * a_weights[3];\n"\
"			return boneTransform;\n"\
"		}\n"\
"	#endif\n"\
"	uniform mat4 u_model;\n"\
"	uniform mat3 u_normalMatrix;\n"\
"	uniform mat4 u_viewProjection; \n"\
"	\n"\
"	mat4 GetModelMatrix()\n"\
"	{\n"\
"		#if defined(SKINNED)\n"\
"			#if defined(INSTANCED)\n"\
"				// See Graphics::UpdateBatchBuffer\n"\
"				// Since we are using rows instead of cols the instancing model matrix is a transpose, \n"\
"				// so the matrix multiply order must be swapped\n"\
"				const vec4 lastColumn = vec4(0.0, 0.0, 0.0, 1.0);\n"\
"			    return GetSkinnedMatrix() * mat4(a_mMatrixRow0, a_mMatrixRow1, a_mMatrixRow2, lastColumn);\n"\
"		    #else\n"\
"				return u_model * GetSkinnedMatrix();\n"\
"			#endif\n"\
"		#elif defined(INSTANCED)\n"\
"			// See Graphics::UpdateBatchBuffer\n"\
"			// Since we are using rows instead of cols the instancing model matrix is a transpose, \n"\
"			// so the matrix multiply order must be swapped\n"\
"			const vec4 lastColumn = vec4(0.0, 0.0, 0.0, 1.0);\n"\
"		    return mat4(a_mMatrixRow0, a_mMatrixRow1, a_mMatrixRow2, lastColumn);\n"\
"	    #else\n"\
"			return u_model;\n"\
"		#endif\n"\
"	}\n"\
"	mat3 GetNormalMatrix()\n"\
"	{\n"\
"		//The normal matrix is used to allow non-uniform scales (sx != sy != sz) in the active node.\n"\
"		//The normal matrix is used to transforms the normal vector from object local space to world space.\n"\
"		//The normal matrix is the transpose of the inverse of the Model matrix.\n"\
"		//For better explanation see http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/\n"\
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
"			return (vec4(a_position, 1.0) * GetModelMatrix()).xyz;\n"\
"		#else\n"\
"	    	return (GetModelMatrix() * vec4(a_position, 1.0)).xyz;\n"\
"	    #endif\n"\
"	}\n"\
"	vec3 GetWorldNormal()\n"\
"	{\n"\
"		#if defined(SKINNED)\n"\
"			//Be careful, bones don't have normal matrix so their scale must be uniform (sx == sy == sz)\n"\
"			#if defined(INSTANCED)\n"\
"				return (vec4(a_normal, 0.0) * GetModelMatrix()).xyz;\n"\
"			#else\n"\
"				return (GetModelMatrix() * vec4(a_normal, 0.0)).xyz;\n"\
"			#endif\n"\
"		#else\n"\
"			return normalize((GetNormalMatrix() * a_normal));\n"\
"		#endif\n"\
"	}\n"\
"	vec3 GetWorldTangent()\n"\
"	{   \n"\
"		#if defined(SKINNED)\n"\
"			//Be careful, bones don't have normal matrix so their scale must be uniform (sx == sy == sz)\n"\
"			#if defined(INSTANCED)\n"\
"				return (vec4(a_tangent, 0.0) * GetModelMatrix()).xyz;\n"\
"			#else\n"\
"				return (GetModelMatrix() * vec4(a_tangent, 0.0)).xyz;\n"\
"			#endif\n"\
"		#else\n"\
"			return normalize((GetNormalMatrix() * a_tangent));\n"\
"		#endif\n"\
"	}\n"\
"	vec4 GetClipPos(vec3 worldPos)\n"\
"	{\n"\
"	    return u_viewProjection * vec4(worldPos, 1.0);\n"\
"	}\n"\
"#endif\n"\
;
}
