//Remember to rebuild with CMake if this file changes
#ifdef COMPILEVS
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	// Vertex shader specific
	/////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////
	attribute vec3 a_position;
	attribute vec2 a_texcoord;
	attribute vec3 a_normal;
	attribute vec4 a_color;
	attribute vec3 a_tangent;
	#if defined(INSTANCED)
	    attribute vec4 a_mMatrixRow0;
	    attribute vec4 a_mMatrixRow1;
	    attribute vec4 a_mMatrixRow2;
	    attribute vec3 a_normalMatrixCol0;
	    attribute vec3 a_normalMatrixCol1;
	    attribute vec3 a_normalMatrixCol2;
	#endif

	uniform mat4 u_model;
	uniform mat3 u_normalMatrix;
	uniform mat4 u_viewProjection; 
	
	mat4 GetModelMatrix()
	{
		#if defined(INSTANCED)
			// Since we are using rows instead of cols the instancing model matrix is a transpose, 
			// so the matrix multiply order must be swapped
			const vec4 lastColumn = vec4(0.0, 0.0, 0.0, 1.0);
		    return mat4(a_mMatrixRow0, a_mMatrixRow1, a_mMatrixRow2, lastColumn);
	    #else
		    return u_model;
	    #endif
	}

	mat3 GetNormalMatrix()
	{
		#if defined(INSTANCED)
			return mat3(a_normalMatrixCol0, a_normalMatrixCol1, a_normalMatrixCol2);
		#else
			return u_normalMatrix;
		#endif
	}

	vec3 GetWorldPos()
	{
		#if defined(INSTANCED)
			// Instancing model matrix is a transpose, so the matrix multiply order must be swapped
			return (vec4(a_position, 1.0) * GetModelMatrix()).xyz;
		#else
	    	return (GetModelMatrix() * vec4(a_position, 1.0)).xyz;
	    #endif
	}

	vec3 GetWorldNormal()
	{
		return normalize((GetNormalMatrix() * a_normal));
	}

	vec3 GetWorldTangent()
	{   
		return normalize((GetNormalMatrix() * a_tangent));
	}

	vec4 GetClipPos(vec3 worldPos)
	{
	    return u_viewProjection * vec4(worldPos, 1.0);
	}

#endif