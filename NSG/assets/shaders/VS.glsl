//Remember to rebuild with CMake if this file changes
#if defined(COMPILEVS) && !defined(HAS_USER_VERTEX_SHADER)
	void main()
	{
		#ifdef SHOW_TEXTURE

			gl_Position = vec4(a_position, 1.0);
			v_texcoord = vec2(a_texcoord.x, 1.0 - a_texcoord.y);

		#elif defined(BLUR) || defined(BLEND)

			gl_Position = vec4(a_position, 1.0);;
			v_texcoord = a_texcoord;

		#elif defined(STENCIL)

			gl_Position = GetClipPos(GetWorldPos());	

		#else

			vec3 worldPos = GetWorldPos();

			#if defined(PER_VERTEX_LIGHTING)

			    vec3 normal = GetWorldNormal();
			    vec3 vertexToEye = normalize(u_eyeWorldPos - worldPos);
			    vec4 totalLight = CalcVSTotalLight(worldPos, vertexToEye, normal);
			    v_color = a_color * totalLight;

			#elif defined(PER_PIXEL_LIGHTING)

				v_normal = GetWorldNormal();
				
				#ifdef NORMAL_MAP					
					v_tangent = GetWorldTangent();
				    v_tangent = normalize(v_tangent - dot(v_tangent, v_normal) * v_normal);
				    v_bitangent = cross(v_tangent, v_normal);
				#endif

				v_color = a_color;				
				v_vertexToEye = normalize(u_eyeWorldPos - worldPos);

				for (int i = 0 ; i < NUM_POINT_LIGHTS ; i++) 
					v_lightDirection[i] = worldPos - u_pointLights[i].position;

				for (int i = 0 ; i < NUM_SPOT_LIGHTS ; i++) 
					v_light2Pixel[i] = worldPos - u_spotLights[i].point.position;
			#else

				v_color = u_material.color * a_color;

			#endif

		    v_texcoord = a_texcoord;
			gl_Position = GetClipPos(worldPos);

		#endif
	}
#endif