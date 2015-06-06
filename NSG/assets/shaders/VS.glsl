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
		#if defined(AMBIENT_PASS)

			#if defined(TEXT)

				v_color = u_material.color * a_color;
				gl_Position = GetClipPos();
				v_texcoord0 = GetTexCoord(a_texcoord0);

			#elif defined(BLUR) || defined(BLEND) || defined(WAVE)

				gl_Position = vec4(a_position, 1.0);
				v_texcoord0 = GetTexCoord(a_texcoord0);

			#elif defined(SHOW_TEXTURE0)

				gl_Position = vec4(a_position, 1.0);
				v_texcoord0 = GetTexCoord(a_texcoord0);

			#elif defined(UNLIT)

				gl_Position = GetClipPos();
				v_texcoord0 = GetTexCoord(a_texcoord0);

			#elif defined(VERTEXCOLOR)
				
				v_color = a_color;
				gl_Position = GetClipPos();

			#else // AMBIENT
			
				gl_Position = GetClipPos();
				v_texcoord0 = GetTexCoord(a_texcoord0);
				#if defined(AOMAP1) || defined(LIGHTMAP1)
					v_texcoord1 = GetTexCoord(a_texcoord1);
				#endif

			#endif

		#elif defined(SHADOWCUBE_PASS) || defined(SHADOW_PASS)

			gl_Position = GetClipPos();
			v_worldPos = GetWorldPos().xyz;

		#else // LIT_PASS

			#if defined(PER_VERTEX_LIGHTING)

				vec4 worldPos = GetWorldPos();
			    vec3 normal = GetWorldNormal();
			    vec3 vertexToEye = normalize(u_eyeWorldPos - worldPos.xyz);
			    vec4 totalLight = CalcTotalLight(worldPos.xyz, vertexToEye, normal);
			    v_color = totalLight;
				v_texcoord0 = GetTexCoord(a_texcoord0);
				gl_Position = GetClipPos();

			#else // PER_PIXEL_LIGHTING

				//Lighting is calculated in world space

				vec4 worldPos = GetWorldPos();
				v_vertexToEye = normalize(u_eyeWorldPos - worldPos.xyz);
				v_normal = GetWorldNormal();
				
				#if defined(NORMALMAP)
					v_tangent = GetWorldTangent(); // Transform the tangent vector to world space (and pass it to the fragment shader).
				    v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);
				    v_bitangent = cross(v_tangent, v_normal);
				    // v_normal, v_tangent and v_bitangent are in world coordinates
				#endif

				#if defined(SHADOWMAP)
					v_shadowClipPos = GetShadowClipPos();
				#endif

				#ifdef HAS_POINT_LIGHT

					v_lightDirection = worldPos.xyz - u_pointLight.position;

				#elif defined(HAS_SPOT_LIGHT)

					v_lightDirection = worldPos.xyz - u_spotLight.position;

				#elif defined(HAS_DIRECTIONAL_LIGHT)

					v_lightDirection = worldPos.xyz - u_directionalLight.position;
						
				#endif

				v_texcoord0 = GetTexCoord(a_texcoord0);
				gl_Position = GetClipPos();

			#endif

		#endif
	}
#endif