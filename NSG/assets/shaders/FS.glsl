//Remember to rebuild with CMake if this file changes
#if defined(COMPILEFS) && !defined(HAS_USER_FRAGMENT_SHADER)

	void main()
	{
		#if defined(VERTEXCOLOR)

			gl_FragColor = v_color;

		#elif defined(UNLIT)

            #ifdef DIFFUSEMAP
				gl_FragColor = texture2D(u_texture0, v_texcoord0);
            #else
                gl_FragColor = u_material.diffuse;
            #endif

		#elif defined(TEXT)

			gl_FragColor = v_color * vec4(vec3(1.0), texture2D(u_texture0, v_texcoord0).a);

		#elif defined(BLEND)

			gl_FragColor = Blend();

		#elif defined(BLUR)

			gl_FragColor = Blur();

		#elif defined(WAVE)

			gl_FragColor = Wave();

		#elif defined(SHOW_TEXTURE0)

			gl_FragColor = texture2D(u_texture0, v_texcoord0);

		#elif defined(AMBIENT)
			
			gl_FragColor = GetAmbientLight();

		#elif defined(SHADOWCUBE_PASS) || defined(SHADOW_PASS)

			vec3 lightToVertex = v_worldPos - u_eyeWorldPos;
    		float lightToPixelDistance = length(lightToVertex) * GetLightInvRange();
    		gl_FragColor = EncodeDepth2Color(lightToPixelDistance);


    	#elif defined(PER_VERTEX_LIGHTING)

			#ifdef DIFFUSEMAP
				gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);
			#else
				gl_FragColor = v_color;
			#endif

		#elif defined(PER_PIXEL_LIGHTING)

				//Lighting is calculated in world space
				#ifdef NORMALMAP
					//The normals in the map are stored in tangent/texture space.
					//For better explanation see http://ogldev.atspace.co.uk/www/tutorial26/tutorial26.html
					//We need to transform them to world space:
					mat3 TBN = mat3(v_tangent, v_bitangent, v_normal); //Generate a world space transformation matrix using the tangent-bitangent-normal.
					vec3 bumpMapNormal = texture2D(u_texture1, v_texcoord0).xyz; // Sample the normal from the normal map 
				    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0); //Normalize from 0..1 to -1..1
				    vec3 normal = normalize(TBN * bumpMapNormal); //Transform the normal from tangent/texture space to world space using the TBN matrix
				#else
					vec3 normal = normalize(v_normal);
				#endif

	    		vec3 vertexToEye = normalize(v_vertexToEye);
	    		vec3 world2light = v_worldPos - GetLightPosition();
	    		vec4 totalLight = CalcTotalLight(world2light, vertexToEye, normal);

				#ifdef DIFFUSEMAP
					vec4 diffuseMap = texture2D(u_texture0, v_texcoord0);
		    		gl_FragColor = totalLight * vec4(diffuseMap.rgb, diffuseMap.a + u_material.diffuse.a);
		    	#else
		    		gl_FragColor = totalLight * u_material.diffuse;
		    	#endif

		#endif	    
	}	

#endif