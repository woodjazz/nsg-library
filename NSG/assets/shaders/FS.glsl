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

		#elif defined(SHADOW_POINT_PASS)

			vec3 lightToVertex = v_worldPos - u_lightPosition;
    		float lightToPixelDistance = length(lightToVertex) * u_lightInvRange;
    		gl_FragColor = EncodeDepth2Color(lightToPixelDistance);

    	#elif defined(SHADOW_DIR_PASS) || defined(SHADOW_SPOT_PASS)

	   		gl_FragColor = EncodeDepth2Color(gl_FragCoord.z);

		#elif defined(LIT)

				//Lighting is calculated in world space
				vec3 normal = GetNormal();
	    		vec3 vertexToEye = normalize(v_vertexToEye);
	    		vec3 world2light = v_worldPos - u_lightPosition;
	    		vec4 finalColor = CalcTotalLight(world2light, vertexToEye, normal);
				#ifdef FOG
					gl_FragColor = vec4(GetLitFog(finalColor.rgb), GetDiffuseColor().a);
				#else
			    	gl_FragColor = vec4(finalColor.rgb, GetDiffuseColor().a);
			    #endif

		#elif defined(IMGUI)

			gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);

		#endif	    
	}	

#endif