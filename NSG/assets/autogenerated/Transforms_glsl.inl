#pragma once
namespace NSG
{
static const char* TRANSFORMS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#ifdef COMPILEVS\n"\
"	// Vertex shader specific\n"\
"	attribute vec3 a_position;\n"\
"	attribute vec2 a_texcoord0;\n"\
"	attribute vec2 a_texcoord1;\n"\
"	attribute vec3 a_normal;\n"\
"	attribute vec4 a_color;\n"\
"	attribute vec3 a_tangent;\n"\
"	attribute vec4 a_boneIDs;\n"\
"	attribute vec4 a_weights;\n"\
"	#if defined(INSTANCED)\n"\
"		attribute vec4 a_mMatrixRow0;\n"\
"		attribute vec4 a_mMatrixRow1;\n"\
"		attribute vec4 a_mMatrixRow2;\n"\
"		attribute vec3 a_normalMatrixCol0;\n"\
"		attribute vec3 a_normalMatrixCol1;\n"\
"		attribute vec3 a_normalMatrixCol2;\n"\
"		mat4 Transpose(mat4 m) \n"\
"		{\n"\
"			vec4 i0 = m[0];\n"\
"			vec4 i1 = m[1];\n"\
"			vec4 i2 = m[2];\n"\
"			vec4 i3 = m[3];\n"\
"			return mat4(\n"\
"				vec4(i0.x, i1.x, i2.x, i3.x),\n"\
"		        vec4(i0.y, i1.y, i2.y, i3.y),\n"\
"		        vec4(i0.z, i1.z, i2.z, i3.z),\n"\
"		        vec4(i0.w, i1.w, i2.w, i3.w)\n"\
"		    );\n"\
"		}	\n"\
"		mat4 GetInstancedMatrix()\n"\
"		{\n"\
"		    // See Graphics::UpdateBatchBuffer\n"\
"		    // Since we are using rows instead of cols the instancing model matrix is a transpose\n"\
"		    const vec4 lastColumn = vec4(0.0, 0.0, 0.0, 1.0);\n"\
"		    return Transpose(mat4(a_mMatrixRow0, a_mMatrixRow1, a_mMatrixRow2, lastColumn));\n"\
"		}\n"\
"	#endif\n"\
"	#if defined(SPHERICAL_BILLBOARD)\n"\
"		mat4 GetSphericalBillboardMatrix(mat4 m)\n"\
"		{\n"\
"			m[0] = vec4(1.0, 0.0, 0.0, m[0][3]);\n"\
"			m[1] = vec4(0.0, 1.0, 0.0, m[1][3]);\n"\
"			m[2] = vec4(0.0, 0.0, 1.0, m[2][3]);\n"\
"			return m;\n"\
"		}\n"\
"	#endif\n"\
"	#if defined(CYLINDRICAL_BILLBOARD)\n"\
"		mat4 GetCylindricalBillboardMatrix(mat4 m)\n"\
"		{\n"\
"			m[0] = vec4(1.0, 0.0, 0.0, m[0][3]);\n"\
"			m[2] = vec4(0.0, 0.0, 1.0, m[2][3]);\n"\
"			return m;\n"\
"		}\n"\
"	#endif	\n"\
"	#if defined(SKINNED)\n"\
"		mat4 GetSkinnedMatrix()\n"\
"		{\n"\
"		    return u_bones[int(a_boneIDs[0])] * a_weights[0] +\n"\
"		    u_bones[int(a_boneIDs[1])] * a_weights[1] +\n"\
"		    u_bones[int(a_boneIDs[2])] * a_weights[2] +\n"\
"		    u_bones[int(a_boneIDs[3])] * a_weights[3];\n"\
"		}\n"\
"	#endif\n"\
"	mat4 GetModelMatrix()\n"\
"	{\n"\
"	    #if defined(SKINNED) && defined(INSTANCED)\n"\
"		    return GetInstancedMatrix() * GetSkinnedMatrix();\n"\
"		#elif defined(SKINNED)\n"\
"			return u_model * GetSkinnedMatrix();\n"\
"	    #elif defined(INSTANCED)\n"\
"		    return GetInstancedMatrix();\n"\
"	    #else\n"\
"	    	return u_model;\n"\
"	    #endif\n"\
"	}\n"\
"	mat4 GetViewWorldMatrix()\n"\
"	{\n"\
"		#if defined(SPHERICAL_BILLBOARD)\n"\
"		    return GetSphericalBillboardMatrix(u_view * GetModelMatrix());\n"\
"		#elif defined(CYLINDRICAL_BILLBOARD)\n"\
"		    return GetCylindricalBillboardMatrix(u_view * GetModelMatrix());\n"\
"		#else\n"\
"		    return u_view * GetModelMatrix();\n"\
"		#endif\n"\
"	}\n"\
"	mat3 GetNormalMatrix()\n"\
"	{\n"\
"	    //The normal matrix is used to allow non-uniform scales (sx != sy != sz) in the active node.\n"\
"	    //The normal matrix is used to transforms the normal vector from object local space to world space.\n"\
"	    //The normal matrix is the transpose of the inverse of the Model matrix.\n"\
"	    //For better explanation see http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/\n"\
"	    #if defined(INSTANCED)\n"\
"	    	return mat3(a_normalMatrixCol0, a_normalMatrixCol1, a_normalMatrixCol2);\n"\
"	    #else\n"\
"	    	return u_normalMatrix;\n"\
"	    #endif\n"\
"	}\n"\
"	vec3 GetWorldNormal()\n"\
"	{\n"\
"	    #if defined(SKINNED)\n"\
"		    //Be careful, bones don't have normal matrix so their scale must be uniform (sx == sy == sz)\n"\
"	    	return (GetModelMatrix() * vec4(a_normal, 0.0)).xyz;\n"\
"	    #else\n"\
"	    	return normalize((GetNormalMatrix() * a_normal));\n"\
"	    #endif\n"\
"	}\n"\
"	#if defined(NORMALMAP)\n"\
"		vec3 GetWorldTangent()\n"\
"		{\n"\
"		    #if defined(SKINNED)\n"\
"			    //Be careful, bones don't have normal matrix so their scale must be uniform (sx == sy == sz)\n"\
"		    	return (GetModelMatrix() * vec4(a_tangent, 0.0)).xyz;\n"\
"		    #else\n"\
"		    	return normalize((GetNormalMatrix() * a_tangent));\n"\
"		    #endif\n"\
"		}\n"\
"	#endif\n"\
"	vec4 GetWorldPos()\n"\
"	{\n"\
"		return GetModelMatrix() * vec4(a_position, 1.0);\n"\
"	}\n"\
"	vec4 GetClipPos()\n"\
"	{\n"\
"		#if defined(SPHERICAL_BILLBOARD)\n"\
"		    return u_projection * GetSphericalBillboardMatrix(u_view * GetModelMatrix()) * vec4(a_position, 1.0);\n"\
"		#elif defined(CYLINDRICAL_BILLBOARD)\n"\
"		    return u_projection * GetCylindricalBillboardMatrix(u_view * GetModelMatrix()) * vec4(a_position, 1.0);\n"\
"		#else\n"\
"		    return u_viewProjection * GetModelMatrix() * vec4(a_position, 1.0);\n"\
"		#endif\n"\
"	}\n"\
"	vec4 GetClipPos(vec4 worldPos)\n"\
"	{\n"\
"		#if defined(SPHERICAL_BILLBOARD)\n"\
"		    return u_projection * GetSphericalBillboardMatrix(u_view * GetModelMatrix()) * vec4(a_position, 1.0);\n"\
"		#elif defined(CYLINDRICAL_BILLBOARD)\n"\
"		    return u_projection * GetCylindricalBillboardMatrix(u_view * GetModelMatrix()) * vec4(a_position, 1.0);\n"\
"		#else\n"\
"		    return u_viewProjection * worldPos;\n"\
"		#endif\n"\
"	}\n"\
"	vec2 GetTexCoord(vec2 texCoord)\n"\
"	{\n"\
"		#if defined(FLIP_Y)\n"\
"			return vec2(texCoord.x, 1.0 - texCoord.y) * u_uvTransform.xy + u_uvTransform.zw;\n"\
"		#else\n"\
"			return texCoord.xy * u_uvTransform.xy + u_uvTransform.zw;\n"\
"		#endif\n"\
"	}\n"\
"#elif defined(COMPILEFS)\n"\
"	#if defined(SHADOW_PASS) || defined(SHADOWCUBE_PASS) \n"\
"		// Input depth [0..1]\n"\
"		// Output color [[0..1], [0..1], [0..1]]\n"\
"		vec4 EncodeDepth2Color(float depth)\n"\
"		{\n"\
"			const vec4 bit_shift = vec4(256.0*256.0*256.0, 256.0*256.0, 256.0, 1.0);\n"\
"			const vec4 bit_mask  = vec4(0.0, 1.0/256.0, 1.0/256.0, 1.0/256.0);\n"\
"			vec4 res = fract(depth * bit_shift);\n"\
"			res -= res.xxyz * bit_mask;\n"\
"			return res;\n"\
"		}\n"\
"	#elif defined(SHADOWMAP) || defined(CUBESHADOWMAP)		\n"\
"		// Input color [[0..1], [0..1], [0..1]]\n"\
"		// Output depth [0..1]\n"\
"		float DecodeColor2Depth(vec4 rgba_depth)\n"\
"		{\n"\
"			const vec4 bit_shift = vec4(1.0/(256.0*256.0*256.0), 1.0/(256.0*256.0), 1.0/256.0, 1.0);\n"\
"			float depth = dot(rgba_depth, bit_shift);\n"\
"			return depth;\n"\
"		}\n"\
"	#endif\n"\
"	#if !defined(AMBIENT_PASS)\n"\
"	    int GetSplit()\n"\
"	    {\n"\
"	        vec4 viewPos = u_view * vec4(v_worldPos, 1.0);\n"\
"	        if(-viewPos.z <= u_shadowCameraZFar[0])\n"\
"	            return 0;\n"\
"	        else if(-viewPos.z <= u_shadowCameraZFar[1])\n"\
"	            return 1;\n"\
"	        else if(-viewPos.z <= u_shadowCameraZFar[2])\n"\
"	            return 2;\n"\
"	        else\n"\
"	            return 3;\n"\
"	    }\n"\
"	    float GetLightInvRange()\n"\
"	    {\n"\
"	    	return u_lightInvRange[GetSplit()];\n"\
"	    }\n"\
"	    vec3 GetLightPosition()\n"\
"	    {\n"\
"	        return u_lightPosition[GetSplit()];\n"\
"	    }\n"\
"	#endif\n"\
"	#if !defined(SHADOW_PASS) && !defined(SHADOWCUBE_PASS) \n"\
"		vec4 GetDiffuseColor()\n"\
"		{\n"\
"		#ifdef UNLIT\n"\
"			#ifdef DIFFUSEMAP\n"\
"				vec4 diffuseMap = texture2D(u_texture0, v_texcoord0);\n"\
"				return vec4(diffuseMap.rgb, diffuseMap.a + u_material.diffuseColor.a);\n"\
"			#else\n"\
"				return u_material.diffuseColor;\n"\
"			#endif\n"\
"		#else\n"\
"			#ifdef DIFFUSEMAP\n"\
"				vec4 diffuseMap = texture2D(u_texture0, v_texcoord0);\n"\
"				return u_material.diffuseIntensity * vec4(diffuseMap.rgb, diffuseMap.a + u_material.diffuseColor.a);\n"\
"			#else\n"\
"				return u_material.diffuseIntensity * u_material.diffuseColor;\n"\
"			#endif		\n"\
"		#endif\n"\
"		}\n"\
"		vec4 GetSpecularColor()\n"\
"		{\n"\
"	    #ifdef SPECULARMAP\n"\
"	        vec4 specularMap = texture2D(u_texture2, v_texcoord0);\n"\
"	        return u_material.specularIntensity * vec4(specularMap.rgb, specularMap.a + u_material.specularColor.a);\n"\
"	    #else\n"\
"	        return u_material.specularIntensity * u_material.specularColor;\n"\
"	    #endif\n"\
"		}\n"\
"    #endif\n"\
"#endif\n"\
;
}
