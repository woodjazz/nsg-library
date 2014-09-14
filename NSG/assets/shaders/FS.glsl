//Remember to rebuild with CMake if this file changes
#if defined(COMPILEFS) && !defined(HAS_USER_FRAGMENT_SHADER)
	
	void main()
	{
		#if defined(TEXT)

			gl_FragColor = v_color * vec4(1.0, 1.0, 1.0, texture2D(u_texture0, v_texcoord).a);

		#elif defined(BLEND)

			gl_FragColor = Blend();

		#elif defined(BLUR)

			gl_FragColor = Blur();

		#elif defined(SHOW_TEXTURE)

			gl_FragColor = texture2D(u_texture0, v_texcoord);

		#elif defined(STENCIL)

			gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);

		#elif defined(PER_PIXEL_LIGHTING)
			#ifdef NORMAL_MAP
				vec3 bumpMapNormal = texture2D(u_texture1, v_texcoord).xyz;
			    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0, 1.0, 1.0);
			    mat3 TBN = mat3(v_tangent, v_bitangent, v_normal);
			    vec3 normal = normalize(TBN * bumpMapNormal);
			#else
				vec3 normal = normalize(v_normal);
			#endif
    		vec3 vertexToEye = normalize(v_vertexToEye);
    		vec4 totalLight = CalcDirectionalLight(vertexToEye, normal);
		    for (int i = 0 ; i < u_numPointLights ; i++) 
		    {
		        totalLight += CalcPointLight(i, vertexToEye, normal);                                            
		    }                                                                                       
		    gl_FragColor = v_color * u_material.color * totalLight * texture2D(u_texture0, v_texcoord);

		#else

			gl_FragColor = v_color * texture2D(u_texture0, v_texcoord);

		#endif	    
	}	

#endif