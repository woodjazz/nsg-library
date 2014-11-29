//Remember to rebuild with CMake if this file changes
#if defined(COMPILEVS) && !defined(HAS_USER_VERTEX_SHADER)
	void main()
	{
		#if defined(TEXT)

			v_texcoord0 = a_texcoord0;
			v_color = u_material.color * a_color;
			gl_Position = GetClipPos(GetWorldPos());

		#elif defined(BLUR) || defined(BLEND)

			gl_Position = vec4(a_position, 1.0);
			v_texcoord0 = a_texcoord0;

		#elif defined(SHOW_TEXTURE0)

			gl_Position = vec4(a_position, 1.0);
			v_texcoord0 = a_texcoord0;

		#elif defined(STENCIL)

			gl_Position = GetClipPos(GetWorldPos());

		#elif defined(UNLIT)

			v_color = u_material.color * a_color;
			v_texcoord0 = a_texcoord0;
			gl_Position = GetClipPos(GetWorldPos());

		#elif defined(PER_VERTEX_LIGHTING)

			vec3 worldPos = GetWorldPos();
		    vec3 normal = GetWorldNormal();
		    vec3 vertexToEye = normalize(u_eyeWorldPos - worldPos);
		    vec4 totalLight = CalcVSTotalLight(worldPos, vertexToEye, normal);
		    v_color = a_color * totalLight;
		    v_texcoord0 = a_texcoord0;

		    #if defined(LIGHTMAP)
		    	v_texcoord1 = a_texcoord1;
		    #endif

			gl_Position = GetClipPos(worldPos);

		#elif defined(PER_PIXEL_LIGHTING)

			//Lighting is calculated in world space

			vec3 worldPos = GetWorldPos();
			v_vertexToEye = normalize(u_eyeWorldPos - worldPos);
			v_normal = GetWorldNormal();
			
			#if defined(NORMALMAP) || defined(DISPLACEMENTMAP)
				v_tangent = GetWorldTangent(); // Transform the tangent vector to world space (and pass it to the fragment shader).
			    v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);
			    v_bitangent = cross(v_tangent, v_normal);
			    // v_normal, v_tangent and v_bitangent are in world coordinates

			    #if defined(DISPLACEMENTMAP)

					// transform direction to the camera to tangent space
					v_vertexToEyeInTangentSpace = vec3(
					     dot(v_vertexToEye, v_tangent),
					     dot(v_vertexToEye, v_bitangent),
					     dot(v_vertexToEye, v_normal)
					  );

				#endif

			#endif

			#ifdef HAS_POINT_LIGHTS

				for (int i = 0 ; i < NUM_POINT_LIGHTS ; i++) 
					v_lightDirection[i] = worldPos - u_pointLights[i].position;

			#endif

			#ifdef HAS_SPOT_LIGHTS

				for (int i = 0 ; i < NUM_SPOT_LIGHTS ; i++) 
					v_light2Pixel[i] = worldPos - u_spotLights[i].point.position;
					
			#endif

			v_color = a_color;
			v_texcoord0 = a_texcoord0;

		    #ifdef LIGHTMAP
		    	v_texcoord1 = a_texcoord1;
		    #endif

			gl_Position = GetClipPos(worldPos);

		#elif defined(LIGHTMAP) // lightmap without lighting

		    v_texcoord0 = a_texcoord0;
			v_texcoord1 = a_texcoord1;
			gl_Position = GetClipPos(GetWorldPos());

		#else // Vertex color by default
			v_color = u_material.color * a_color;
			gl_Position = GetClipPos(GetWorldPos());
			
		#endif
	}
#endif