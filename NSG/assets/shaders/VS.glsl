//Remember to rebuild with CMake if this file changes
#if defined(COMPILEVS) && !defined(HAS_USER_VERTEX_SHADER)

	void main()
	{
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

			v_color = u_material.color * a_color;
			gl_Position = GetClipPos();
			v_texcoord0 = GetTexCoord(a_texcoord0);

		#elif defined(LIGHTMAP) // lightmap without lighting

			v_color = u_material.color * a_color;
			v_texcoord0 = GetTexCoord(a_texcoord0);
			v_texcoord1 = GetTexCoord(a_texcoord1);
			
			gl_Position = GetClipPos();

		#elif defined(PER_VERTEX_LIGHTING)

			vec4 worldPos = GetWorldPos();
		    vec3 normal = GetWorldNormal();
		    vec3 vertexToEye = normalize(u_eyeWorldPos - worldPos.xyz);
		    vec4 totalLight = CalcVSTotalLight(worldPos.xyz, vertexToEye, normal);
		    v_color = a_color * totalLight;
			v_texcoord0 = GetTexCoord(a_texcoord0);
			
			#if defined(AOMAP)
				v_texcoord1 = GetTexCoord(a_texcoord1);
			#endif

			gl_Position = GetClipPos(worldPos);

		#elif defined(PER_PIXEL_LIGHTING)

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

			#ifdef HAS_POINT_LIGHT

				v_lightDirection = worldPos.xyz - u_pointLight.position;

			#elif defined(HAS_SPOT_LIGHT)

				v_light2Pixel = worldPos.xyz - u_spotLight.position;
					
			#endif

			v_color = a_color;
			v_texcoord0 = GetTexCoord(a_texcoord0);

			#if defined(AOMAP)
				v_texcoord1 = GetTexCoord(a_texcoord1);
			#endif

			gl_Position = GetClipPos(worldPos);

		#else // Vertex color by default
			v_color = u_material.color * a_color;
			gl_Position = GetClipPos();
			
		#endif
	}
#endif