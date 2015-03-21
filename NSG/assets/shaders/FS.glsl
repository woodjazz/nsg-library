//Remember to rebuild with CMake if this file changes
#if defined(COMPILEFS) && !defined(HAS_USER_FRAGMENT_SHADER)

	#ifdef DISPLACEMENTMAP

		vec2 ParallaxMapping(in vec2 T, in vec3 V, out float parallaxHeight)
		{
			//See http://sunandblackcat.com/tipFullView.php?topicid=28
			
			// determine required number of layers
			const float minLayers = 10.0;
			const float maxLayers = 15.0;
			float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), V)));

			// height of each layer
			float layerHeight = 1.0 / numLayers;
			// depth of current layer
			float currentLayerHeight = 0.0;
			// shift of texture coordinates for each iteration
			vec2 dtex = u_material.parallaxScale * V.xy / V.z / numLayers;

			// current texture coordinates
			vec2 currentTextureCoords = T;

			// depth from heightmap
			float heightFromTexture = 1.0 - texture2D(u_texture5, currentTextureCoords).r;

			const int MAX_LAYERS = 10;
			// while point is above surface
			for(int i=0; i<MAX_LAYERS; i++)
			{
				if(heightFromTexture <= currentLayerHeight) 
					break;
				// go to the next layer
				currentLayerHeight += layerHeight; 
				// shift texture coordinates along V
				currentTextureCoords -= dtex;
				// new depth from heightmap
				heightFromTexture = 1.0 - texture2D(u_texture5, currentTextureCoords).r;
			}

			///////////////////////////////////////////////////////////
			// Start of Relief Parallax Mapping

			// decrease shift and height of layer by half
			vec2 deltaTexCoord = dtex / 2.0;
			float deltaHeight = layerHeight / 2.0;

			// return to the mid point of previous layer
			currentTextureCoords += deltaTexCoord;
			currentLayerHeight -= deltaHeight;

			// binary search to increase precision of Steep Paralax Mapping
			const int NUM_SEARCHES = 5;
			
			for(int i=0; i<NUM_SEARCHES; i++)
			{
				// decrease shift and height of layer by half
				deltaTexCoord /= 2.0;
				deltaHeight /= 2.0;

				// new depth from heightmap
				heightFromTexture = 1.0 - texture2D(u_texture5, currentTextureCoords).r;

				// shift along or agains vector V
				if(heightFromTexture > currentLayerHeight) // below the surface
				{
					currentTextureCoords -= deltaTexCoord;
					currentLayerHeight += deltaHeight;
				}
				else // above the surface
				{
					currentTextureCoords += deltaTexCoord;
					currentLayerHeight -= deltaHeight;
				}
			}

			// return results
			parallaxHeight = currentLayerHeight;
			return currentTextureCoords;
		}	

	#endif

	void main()
	{
		#if defined(TEXT)

			gl_FragColor = v_color * vec4(1.0, 1.0, 1.0, texture2D(u_texture0, v_texcoord0).a);

		#elif defined(BLEND)

			gl_FragColor = Blend();

		#elif defined(BLUR)

			gl_FragColor = Blur();

		#elif defined(WAVE)

			gl_FragColor = Wave();

		#elif defined(SHOW_TEXTURE0)

			gl_FragColor = texture2D(u_texture0, v_texcoord0);

		#elif defined(STENCIL)

			gl_FragColor = vec4(1.0);

		#elif defined(UNLIT)

			#ifdef DIFFUSEMAP
				gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);
			#else
				gl_FragColor = v_color;
			#endif

		#elif defined(PER_VERTEX_LIGHTING)

			#ifdef DIFFUSEMAP
				gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);
			#else
				gl_FragColor = v_color;
			#endif

		#elif defined(PER_PIXEL_LIGHTING)

			//Lighting is calculated in world space

			#ifdef DISPLACEMENTMAP
				float parallaxHeight;
				vec2 texcoord0 = ParallaxMapping(v_texcoord0, normalize(v_vertexToEyeInTangentSpace), parallaxHeight);
			#else
				vec2 texcoord0 = v_texcoord0;
			#endif

			#ifdef NORMALMAP
				//The normals in the map are stored in tangent/texture space.
				//For better explanation see http://ogldev.atspace.co.uk/www/tutorial26/tutorial26.html
				//We need to transform them to world space:
				mat3 TBN = mat3(v_tangent, v_bitangent, v_normal); //Generate a world space transformation matrix using the tangent-bitangent-normal.
				vec3 bumpMapNormal = texture2D(u_texture1, texcoord0).xyz; // Sample the normal from the normal map 
			    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0); //Normalize from 0..1 to -1..1
			    vec3 normal = normalize(TBN * bumpMapNormal); //Transform the normal from tangent/texture space to world space using the TBN matrix
			#else
				vec3 normal = normalize(v_normal);
			#endif

    		vec3 vertexToEye = normalize(v_vertexToEye);
    		vec4 totalLight = CalcFSTotalLight(vertexToEye, normal);

			#ifdef DIFFUSEMAP
	    		gl_FragColor = v_color * totalLight * texture2D(u_texture0, texcoord0);
	    	#else
	    		gl_FragColor = v_color * totalLight;
	    	#endif

		#elif defined(LIGHTMAP) // lightmap without lighting
			
			#ifdef DIFFUSEMAP
				gl_FragColor = v_color * texture2D(u_texture2, v_texcoord1) * texture2D(u_texture0, v_texcoord0);
			#else
				gl_FragColor = v_color * texture2D(u_texture2, v_texcoord1);
			#endif

		#else // Vertex color by default

			gl_FragColor = v_color;

		#endif	    
	}	

#endif