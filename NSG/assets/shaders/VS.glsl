//Remember to rebuild with CMake if this file changes
#if defined(COMPILEVS) && !defined(HAS_USER_VERTEX_SHADER)

	// use always gl_Position = GetClipPos() between different passes
	// to avoid variation and z-fighting issues.
	// For example do not use:
	//		gl_Position = GetClipPos(); for AMBIENT pass
	//		gl_Position = GetClipPos(worldPos); for PER_PIXEL_LIGHTING pass
	// since it can produce different results and cause z-fighting between passes
	void main()
	{
		#if defined(VERTEXCOLOR)
			
			v_color = a_color;
			gl_Position = GetClipPos();

		#elif defined(UNLIT)
			
			//v_color = a_color;
			gl_Position = GetClipPos();
			v_texcoord0 = GetTexCoord(a_texcoord0);

		#elif defined(TEXT)

			v_color = u_material.color * a_color;
			gl_Position = GetClipPos();
			v_texcoord0 = GetTexCoord(a_texcoord0);

		#elif defined(BLUR) || defined(BLEND) || defined(WAVE) || defined(SHOW_TEXTURE0)

			gl_Position = vec4(a_position, 1.0);
			v_texcoord0 = GetTexCoord(a_texcoord0);

		#elif defined(AMBIENT)
		
			gl_Position = GetClipPos();
			v_texcoord0 = GetTexCoord(a_texcoord0);
			#if defined(AOMAP1) || defined(LIGHTMAP1)
				v_texcoord1 = GetTexCoord(a_texcoord1);
			#endif

		#elif defined(SHADOWCUBE_PASS) || defined(SHADOW_PASS)

			gl_Position = GetClipPos();
			v_worldPos = GetWorldPos().xyz;

		#elif defined(PER_VERTEX_LIGHTING)

			v_worldPos = GetWorldPos().xyz;
		    vec3 normal = GetWorldNormal();
		    vec3 vertexToEye = normalize(u_eyeWorldPos - v_worldPos);
		    vec3 world2light = worldPos - u_lightPosition[0];
		    vec4 totalLight = CalcTotalLight(world2light, vertexToEye, normal);
		    v_color = totalLight;
			v_texcoord0 = GetTexCoord(a_texcoord0);
			gl_Position = GetClipPos();

		#elif defined(PER_PIXEL_LIGHTING)

			//Lighting is calculated in world space

			v_worldPos = GetWorldPos().xyz;
			v_vertexToEye = normalize(u_eyeWorldPos - v_worldPos);
			v_normal = GetWorldNormal();
			
			#if defined(NORMALMAP)
				v_tangent = GetWorldTangent(); // Transform the tangent vector to world space (and pass it to the fragment shader).
			    v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);
			    v_bitangent = cross(v_tangent, v_normal);
			    // v_normal, v_tangent and v_bitangent are in world coordinates
			#endif

			v_texcoord0 = GetTexCoord(a_texcoord0);
			gl_Position = GetClipPos();

		#endif
	}
#endif