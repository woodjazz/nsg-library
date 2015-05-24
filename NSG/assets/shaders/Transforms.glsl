//Remember to rebuild with CMake if this file changes
#ifdef COMPILEVS
// Vertex shader specific
attribute vec3 a_position;
attribute vec2 a_texcoord0;
attribute vec2 a_texcoord1;
attribute vec3 a_normal;
attribute vec4 a_color;
attribute vec3 a_tangent;
attribute vec4 a_boneIDs;
attribute vec4 a_weights;

#if defined(INSTANCED)
	attribute vec4 a_mMatrixRow0;
	attribute vec4 a_mMatrixRow1;
	attribute vec4 a_mMatrixRow2;
	attribute vec3 a_normalMatrixCol0;
	attribute vec3 a_normalMatrixCol1;
	attribute vec3 a_normalMatrixCol2;

	mat4 Transpose(mat4 m) 
	{
		vec4 i0 = m[0];
		vec4 i1 = m[1];
		vec4 i2 = m[2];
		vec4 i3 = m[3];
		return mat4(
			vec4(i0.x, i1.x, i2.x, i3.x),
	        vec4(i0.y, i1.y, i2.y, i3.y),
	        vec4(i0.z, i1.z, i2.z, i3.z),
	        vec4(i0.w, i1.w, i2.w, i3.w)
	    );
	}	

	mat4 GetInstancedMatrix()
	{
	    // See Graphics::UpdateBatchBuffer
	    // Since we are using rows instead of cols the instancing model matrix is a transpose

	    const vec4 lastColumn = vec4(0.0, 0.0, 0.0, 1.0);
	    return Transpose(mat4(a_mMatrixRow0, a_mMatrixRow1, a_mMatrixRow2, lastColumn));
	}
#endif

#if defined(SPHERICAL_BILLBOARD)
	mat4 GetSphericalBillboardMatrix(mat4 m)
	{
		m[0] = vec4(1.0, 0.0, 0.0, m[0][3]);
		m[1] = vec4(0.0, 1.0, 0.0, m[1][3]);
		m[2] = vec4(0.0, 0.0, 1.0, m[2][3]);
		return m;
	}
#endif

#if defined(CYLINDRICAL_BILLBOARD)
	mat4 GetCylindricalBillboardMatrix(mat4 m)
	{
		m[0] = vec4(1.0, 0.0, 0.0, m[0][3]);
		m[2] = vec4(0.0, 0.0, 1.0, m[2][3]);
		return m;
	}
#endif	

#if defined(SKINNED)
	uniform mat4 u_bones[MAX_BONES];
	mat4 GetSkinnedMatrix()
	{
	    return u_bones[int(a_boneIDs[0])] * a_weights[0] +
	    u_bones[int(a_boneIDs[1])] * a_weights[1] +
	    u_bones[int(a_boneIDs[2])] * a_weights[2] +
	    u_bones[int(a_boneIDs[3])] * a_weights[3];
	}
#endif

uniform mat4 u_model;
uniform mat3 u_normalMatrix;
uniform mat4 u_viewProjection;
uniform mat4 u_lightViewProjection;
uniform mat4 u_view;
uniform mat4 u_projection;

mat4 GetModelMatrix()
{
    #if defined(SKINNED) && defined(INSTANCED)
	    return GetInstancedMatrix() * GetSkinnedMatrix();
	#elif defined(SKINNED)
		return u_model * GetSkinnedMatrix();
    #elif defined(INSTANCED)
	    return GetInstancedMatrix();
    #else
    	return u_model;
    #endif
}

mat4 GetViewWorldMatrix()
{
	#if defined(SPHERICAL_BILLBOARD)
	    return GetSphericalBillboardMatrix(u_view * GetModelMatrix());
	#elif defined(CYLINDRICAL_BILLBOARD)
	    return GetCylindricalBillboardMatrix(u_view * GetModelMatrix());
	#else
	    return u_view * GetModelMatrix();
	#endif
}

#if defined(HAS_DIRECTIONAL_LIGHT) || defined(HAS_POINT_LIGHT) || defined(HAS_SPOT_LIGHT)

	mat3 GetNormalMatrix()
	{
	    //The normal matrix is used to allow non-uniform scales (sx != sy != sz) in the active node.
	    //The normal matrix is used to transforms the normal vector from object local space to world space.
	    //The normal matrix is the transpose of the inverse of the Model matrix.
	    //For better explanation see http://www.lighthouse3d.com/tutorials/glsl-tutorial/the-normal-matrix/
	    #if defined(INSTANCED)
	    	return mat3(a_normalMatrixCol0, a_normalMatrixCol1, a_normalMatrixCol2);
	    #else
	    	return u_normalMatrix;
	    #endif
	}

	vec3 GetWorldNormal()
	{
	    #if defined(SKINNED)
		    //Be careful, bones don't have normal matrix so their scale must be uniform (sx == sy == sz)
	    	return (GetModelMatrix() * vec4(a_normal, 0.0)).xyz;
	    #else
	    	return normalize((GetNormalMatrix() * a_normal));
	    #endif
	}

	#if defined(NORMALMAP)
		vec3 GetWorldTangent()
		{
		    #if defined(SKINNED)
			    //Be careful, bones don't have normal matrix so their scale must be uniform (sx == sy == sz)
		    	return (GetModelMatrix() * vec4(a_tangent, 0.0)).xyz;
		    #else
		    	return normalize((GetNormalMatrix() * a_tangent));
		    #endif
		}
	#endif
#endif

vec4 GetWorldPos()
{
	return GetModelMatrix() * vec4(a_position, 1.0);
}

vec4 GetClipPos()
{
	#if defined(SPHERICAL_BILLBOARD)
	    return u_projection * GetSphericalBillboardMatrix(u_view * GetModelMatrix()) * vec4(a_position, 1.0);
	#elif defined(CYLINDRICAL_BILLBOARD)
	    return u_projection * GetCylindricalBillboardMatrix(u_view * GetModelMatrix()) * vec4(a_position, 1.0);
	#else
	    return u_viewProjection * GetModelMatrix() * vec4(a_position, 1.0);
	#endif
}

vec4 GetClipPos(vec4 worldPos)
{
	#if defined(SPHERICAL_BILLBOARD)
	    return u_projection * GetSphericalBillboardMatrix(u_view * GetModelMatrix()) * vec4(a_position, 1.0);
	#elif defined(CYLINDRICAL_BILLBOARD)
	    return u_projection * GetCylindricalBillboardMatrix(u_view * GetModelMatrix()) * vec4(a_position, 1.0);
	#else
	    return u_viewProjection * worldPos;
	#endif
}

vec4 GetShadowClipPos()
{
	return u_lightViewProjection * GetModelMatrix() * vec4(a_position, 1.0);
}

uniform vec4 u_uvTransform;
vec2 GetTexCoord(vec2 texCoord)
{
	#if defined(FLIP_Y)
		return vec2(texCoord.x, 1.0 - texCoord.y) * u_uvTransform.xy + u_uvTransform.zw;
	#else
		return texCoord.xy * u_uvTransform.xy + u_uvTransform.zw;
	#endif
}

#elif defined(COMPILEFS)

	#if defined(SHADOW) || defined(SHADOWCUBE)

		// Input depth [-1..1] (NDC space)
		// Output color [[0..1], [0..1], [0..1]]
		#if 1
		vec3 EncodeDepth2Color(float depth)
		{
			float DISTANCE = 1.0/u_lightInvRange;
			float value = DISTANCE - depth;
			float v = floor(value);
			float f = value - v;
			float vn = v * 1.0/DISTANCE;
			return vec3(vn, f, 0.0);
		}
		#else
		vec3 EncodeDepth2Color(float depth)
		{
			depth = 0.5 * depth + 0.5;
		    vec3 ret;
		    depth *= 255.0;
		    ret.x = floor(depth);
		    depth = (depth - ret.x) * 255.0;
		    ret.y = floor(depth);
		    ret.z = (depth - ret.y);
		    ret.xy *= 1.0 / 255.0;
		    return ret;
		}
		#endif

	#elif defined(SHADOWMAP) || defined(CUBESHADOWMAP)		

		// Input color [[0..1], [0..1], [0..1]]
		// Output depth [-1..1] (NDC space)
		#if 1
		float DecodeColor2Depth(vec3 depth)
		{
			float DISTANCE = 1.0/u_lightInvRange;
			return DISTANCE - (depth.x * DISTANCE + depth.y);
		}
		#else
		float DecodeColor2Depth(vec3 depth)
		{
		    const vec3 dotValues = vec3(1.0, 1.0 / 255.0, 1.0 / (255.0 * 255.0));
		    float value = dot(depth, dotValues);
		    value = 2.0 * (value - 0.5);
		    return value;
		}
		#endif

	#endif

#endif