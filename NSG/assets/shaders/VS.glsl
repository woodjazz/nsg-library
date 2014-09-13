//Remember to rebuild with CMake if this file changes
#if defined(COMPILEVS) && !defined(HAS_USER_VERTEX_SHADER)
	void main()
	{
		#ifdef SHOW_TEXTURE

			gl_Position = a_position;
			v_texcoord = vec2(a_texcoord.x, 1.0 - a_texcoord.y);

		#elif defined(BLUR) || defined(BLEND)

			gl_Position = a_position;
			v_texcoord = a_texcoord;

		#elif defined(STENCIL)

			gl_Position = GetClipPos(GetWorldPos());	

		#else

			vec3 worldPos = GetWorldPos();

			#if defined(PER_VERTEX_LIGHTING)

			    vec3 normal = GetWorldNormal();
			    v_vertexToEye = normalize(u_eyeWorldPos - worldPos);
			    vec4 totalLight = CalcDirectionalLight(v_vertexToEye, normal);
			    for (int i = 0 ; i < u_numPointLights ; i++) 
			    {
			    	v_lightDirection[i] = worldPos - u_pointLights[i].position;
			        totalLight += CalcPointLight(i, v_vertexToEye, normal);                                            
			    }                                                                                       
			    v_color = a_color * totalLight;

			#elif defined(PER_PIXEL_LIGHTING)

				v_color = a_color;
				v_normal = GetWorldNormal();
				v_vertexToEye = normalize(u_eyeWorldPos - worldPos);
				for (int i = 0 ; i < u_numPointLights ; i++) 
				{
					v_lightDirection[i] = worldPos - u_pointLights[i].position;
				}

			#else

				v_color = u_material.color * a_color;

			#endif

		    v_texcoord = a_texcoord;
			gl_Position = GetClipPos(worldPos);

		#endif
	}
#endif