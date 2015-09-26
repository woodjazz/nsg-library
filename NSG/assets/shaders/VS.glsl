//Remember to rebuild with CMake if this file changes
#if defined(COMPILEVS) && !defined(HAS_USER_VERTEX_SHADER)

	// use always gl_Position = GetClipPos() between different passes
	// to avoid variation and z-fighting issues.
	// For example do not use:
	//		gl_Position = GetClipPos(); for AMBIENT pass
	//		gl_Position = GetClipPos(worldPos); for LIT pass
	// since it can produce different results and cause z-fighting between passes

	void main()
	{
		#if defined(VERTEXCOLOR)
			
			v_color = a_color;
			gl_Position = GetClipPos();

		#elif defined(TEXT)

			gl_Position = GetClipPos();
			v_texcoord0 = GetTexCoord(a_texcoord0, u_uvTransform0);

		#elif defined(BLUR) || defined(BLEND) || defined(WAVE) || defined(SHOW_TEXTURE0)

			gl_Position = vec4(a_position, 1.0);
			v_texcoord0 = GetTexCoord(a_texcoord0, u_uvTransform0);

		#elif defined(AMBIENT) || defined(UNLIT)
		
			gl_Position = GetClipPos();
			v_texcoord0 = GetTexCoord(a_texcoord0, u_uvTransform0);
			v_texcoord1 = GetTexCoord(a_texcoord1, u_uvTransform1);

			#ifdef FOG
				v_worldPos = GetWorldPos().xyz;
				v_depth = -GetCameraPos().z;
			#endif

		#elif defined(SHADOW_POINT_PASS) || defined(SHADOW_DIR_PASS) || defined(SHADOW_SPOT_PASS)

			gl_Position = GetClipPos();
			v_worldPos = GetWorldPos().xyz;

		#elif defined(LIT)

			//Lighting is calculated in world space

			v_worldPos = GetWorldPos().xyz;
			v_vertexToEye = normalize(u_eyeWorldPos - v_worldPos);
			v_normal = GetWorldNormal();

			#ifdef FOG
				v_depth = -GetCameraPos().z;
			#endif
			
			#if defined(NORMALMAP0) || defined(NORMALMAP1)
				v_tangent = GetWorldTangent(); // Transform the tangent vector to world space (and pass it to the fragment shader).
			    v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);
			    v_bitangent = cross(v_tangent, v_normal);
			    // v_normal, v_tangent and v_bitangent are in world coordinates
			#endif

			v_texcoord0 = GetTexCoord(a_texcoord0, u_uvTransform0);
			v_texcoord1 = GetTexCoord(a_texcoord1, u_uvTransform1);
			gl_Position = GetClipPos();

		#elif defined(IMGUI)

			v_color = a_color;
			v_texcoord0 = a_texcoord0;
			gl_Position = u_viewProjection * vec4(a_position.xy, 0.0, 1.0);

		#endif
	}
#endif