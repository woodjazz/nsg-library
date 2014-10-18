//Remember to rebuild with CMake if this file changes
#if defined(COMPILEFS) && !defined(HAS_USER_FRAGMENT_SHADER)
	
	void main()
	{
		#if defined(TEXT)

			gl_FragColor = v_color * vec4(1.0, 1.0, 1.0, texture2D(u_texture0, v_texcoord0).a);

		#elif defined(BLEND)

			gl_FragColor = Blend();

		#elif defined(BLUR)

			gl_FragColor = Blur();

		#elif defined(SHOW_TEXTURE0)

			gl_FragColor = texture2D(u_texture0, v_texcoord0);

		#elif defined(STENCIL)

			gl_FragColor = vec4(1.0);

		#elif defined(UNLIT)

			gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);

		#elif defined(LIGHTMAP)
			
			gl_FragColor = texture2D(u_texture2, v_texcoord1) * texture2D(u_texture0, v_texcoord0);

		#elif defined(PER_VERTEX_LIGHTING)

			gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);

		#elif defined(PER_PIXEL_LIGHTING)

			#ifdef NORMALMAP
				vec3 bumpMapNormal = texture2D(u_texture1, v_texcoord0).xyz;
			    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0);
			    mat3 TBN = mat3(v_tangent, v_bitangent, v_normal);
			    vec3 normal = normalize(TBN * bumpMapNormal);
			#else
				vec3 normal = normalize(v_normal);
			#endif

    		vec3 vertexToEye = normalize(v_vertexToEye);
    		vec4 totalLight = CalcFSTotalLight(vertexToEye, normal);
		    gl_FragColor = v_color * totalLight * texture2D(u_texture0, v_texcoord0);

		#else // Vertex color by default

			gl_FragColor = v_color;

		#endif	    
	}	

#endif