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

		#elif defined(SHADOWCUBE_PASS) || defined(SHADOW_PASS_SPOT)

			vec3 lightToVertex = v_worldPos - u_lightPosition;
    		float lightToPixelDistance = length(lightToVertex) * u_lightInvRange;
    		gl_FragColor = EncodeDepth2Color(lightToPixelDistance);

    	#elif defined(SHADOW_PASS)

			vec3 lightToVertex = v_worldPos - GetShadowCamPos();
    		float lightToPixelDistance = length(lightToVertex) * GetShadowCamInvRange();
    		gl_FragColor = EncodeDepth2Color(lightToPixelDistance);

    		#if 0
	        vec4 shadowClipPos = GetShadowClipPos(vec4(v_worldPos, 1.0));
	        vec4 coords = shadowClipPos / shadowClipPos.w; // Normalize from -w..w to -1..1
	        coords = 0.5 * coords + 0.5; // Normalize from -1..1 to 0..1
	        gl_FragColor = EncodeDepth2Color(clamp(coords.z, 0.0, 1.0));
	        #endif

    	#elif defined(PER_VERTEX_LIGHTING)

			gl_FragColor = v_color;// * GetDiffuseColor();

		#elif defined(PER_PIXEL_LIGHTING)

				//Lighting is calculated in world space
				vec3 normal = GetNormal();
	    		vec3 vertexToEye = normalize(v_vertexToEye);
	    		vec3 world2light = v_worldPos - GetShadowCamPos();
				#ifdef FOG
					vec4 finalColor = CalcTotalLight(world2light, vertexToEye, normal);
					gl_FragColor = vec4(GetLitFog(finalColor.rgb), finalColor.a);
				#else
			    	gl_FragColor = CalcTotalLight(world2light, vertexToEye, normal);
			    #endif

		#elif defined(IMGUI)

			gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);

		#endif	    
	}	

#endif