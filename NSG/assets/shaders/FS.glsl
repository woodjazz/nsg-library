//Remember to rebuild with CMake if this file changes
#if defined(COMPILEFS) && !defined(HAS_USER_FRAGMENT_SHADER)

	void main()
	{
		#if defined(VERTEXCOLOR)

			gl_FragColor = v_color;

		#elif defined(TEXT)

			gl_FragColor = vec4(vec3(u_material.diffuseColor), GetDiffuseColor().a);

		#elif defined(BLEND)

			gl_FragColor = Blend();

		#elif defined(BLUR)

			gl_FragColor = Blur();

		#elif defined(WAVE)

			gl_FragColor = Wave();

		#elif defined(SHOW_TEXTURE0)

			gl_FragColor = texture2D(u_texture0, v_texcoord0);

		#elif defined(AMBIENT) || defined(UNLIT)
			
			#ifdef FOG
				vec4 finalColor = GetAmbientIntensity() * GetDiffuseColor();
				gl_FragColor = vec4(GetAmbientFog(finalColor.rgb), finalColor.a);
			#else
				gl_FragColor = GetAmbientIntensity() * GetDiffuseColor();
			#endif

		#elif defined(SHADOWCUBE_PASS) || defined(SHADOW_PASS)

			vec3 lightToVertex = v_worldPos - u_eyeWorldPos;
    		float lightToPixelDistance = length(lightToVertex) * GetLightInvRange();
    		gl_FragColor = EncodeDepth2Color(lightToPixelDistance);


    	#elif defined(PER_VERTEX_LIGHTING)

			gl_FragColor = v_color * GetDiffuseColor();

		#elif defined(PER_PIXEL_LIGHTING)

				//Lighting is calculated in world space
				vec3 normal = GetNormal();
	    		vec3 vertexToEye = normalize(v_vertexToEye);
	    		vec3 world2light = v_worldPos - GetLightPosition();
				#ifdef FOG
					vec4 finalColor = CalcTotalLight(world2light, vertexToEye, normal) * GetDiffuseColor();
					gl_FragColor = vec4(GetLitFog(finalColor.rgb), finalColor.a);
				#else
			    	gl_FragColor = CalcTotalLight(world2light, vertexToEye, normal) * GetDiffuseColor();
			    #endif

		#endif	    
	}	

#endif