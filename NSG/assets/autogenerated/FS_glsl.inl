#pragma once
namespace NSG
{
static const char* FS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(COMPILEFS) && !defined(HAS_USER_FRAGMENT_SHADER)\n"\
"	#ifdef DISPLACEMENTMAP\n"\
"		vec2 ParallaxMapping(in vec2 T, in vec3 V, out float parallaxHeight)\n"\
"		{\n"\
"			//See http://sunandblackcat.com/tipFullView.php?topicid=28\n"\
"			\n"\
"			// determine required number of layers\n"\
"			const float minLayers = 10.0;\n"\
"			const float maxLayers = 15.0;\n"\
"			float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), V)));\n"\
"			// height of each layer\n"\
"			float layerHeight = 1.0 / numLayers;\n"\
"			// depth of current layer\n"\
"			float currentLayerHeight = 0.0;\n"\
"			// shift of texture coordinates for each iteration\n"\
"			vec2 dtex = u_material.parallaxScale * V.xy / V.z / numLayers;\n"\
"			// current texture coordinates\n"\
"			vec2 currentTextureCoords = T;\n"\
"			// depth from heightmap\n"\
"			float heightFromTexture = 1.0 - texture2D(u_texture5, currentTextureCoords).r;\n"\
"			const int MAX_LAYERS = 10;\n"\
"			// while point is above surface\n"\
"			for(int i=0; i<MAX_LAYERS; i++)\n"\
"			{\n"\
"				if(heightFromTexture <= currentLayerHeight) \n"\
"					break;\n"\
"				// go to the next layer\n"\
"				currentLayerHeight += layerHeight; \n"\
"				// shift texture coordinates along V\n"\
"				currentTextureCoords -= dtex;\n"\
"				// new depth from heightmap\n"\
"				heightFromTexture = 1.0 - texture2D(u_texture5, currentTextureCoords).r;\n"\
"			}\n"\
"			///////////////////////////////////////////////////////////\n"\
"			// Start of Relief Parallax Mapping\n"\
"			// decrease shift and height of layer by half\n"\
"			vec2 deltaTexCoord = dtex / 2.0;\n"\
"			float deltaHeight = layerHeight / 2.0;\n"\
"			// return to the mid point of previous layer\n"\
"			currentTextureCoords += deltaTexCoord;\n"\
"			currentLayerHeight -= deltaHeight;\n"\
"			// binary search to increase precision of Steep Paralax Mapping\n"\
"			const int NUM_SEARCHES = 5;\n"\
"			\n"\
"			for(int i=0; i<NUM_SEARCHES; i++)\n"\
"			{\n"\
"				// decrease shift and height of layer by half\n"\
"				deltaTexCoord /= 2.0;\n"\
"				deltaHeight /= 2.0;\n"\
"				// new depth from heightmap\n"\
"				heightFromTexture = 1.0 - texture2D(u_texture5, currentTextureCoords).r;\n"\
"				// shift along or agains vector V\n"\
"				if(heightFromTexture > currentLayerHeight) // below the surface\n"\
"				{\n"\
"					currentTextureCoords -= deltaTexCoord;\n"\
"					currentLayerHeight += deltaHeight;\n"\
"				}\n"\
"				else // above the surface\n"\
"				{\n"\
"					currentTextureCoords += deltaTexCoord;\n"\
"					currentLayerHeight -= deltaHeight;\n"\
"				}\n"\
"			}\n"\
"			// return results\n"\
"			parallaxHeight = currentLayerHeight;\n"\
"			return currentTextureCoords;\n"\
"		}	\n"\
"	#endif\n"\
"	void main()\n"\
"	{\n"\
"		#if defined(TEXT)\n"\
"			gl_FragColor = v_color * vec4(1.0, 1.0, 1.0, texture2D(u_texture0, v_texcoord0).r);\n"\
"		#elif defined(BLEND)\n"\
"			gl_FragColor = Blend();\n"\
"		#elif defined(BLUR)\n"\
"			gl_FragColor = Blur();\n"\
"		#elif defined(SHOW_TEXTURE0)\n"\
"			gl_FragColor = texture2D(u_texture0, v_texcoord0);\n"\
"		#elif defined(STENCIL)\n"\
"			gl_FragColor = vec4(1.0);\n"\
"		#elif defined(UNLIT)\n"\
"			#ifdef DIFFUSEMAP\n"\
"				gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);\n"\
"			#else\n"\
"				gl_FragColor = v_color;\n"\
"			#endif\n"\
"		#elif defined(PER_VERTEX_LIGHTING)\n"\
"			#ifdef LIGHTMAP\n"\
"				#ifdef DIFFUSEMAP\n"\
"					gl_FragColor = v_color * texture2D(u_texture2, v_texcoord1) * texture2D(u_texture0, v_texcoord0);\n"\
"				#else\n"\
"					gl_FragColor = v_color * texture2D(u_texture2, v_texcoord1);\n"\
"				#endif\n"\
"			#else\n"\
"				#ifdef DIFFUSEMAP\n"\
"					gl_FragColor = v_color * texture2D(u_texture0, v_texcoord0);\n"\
"				#else\n"\
"					gl_FragColor = v_color;\n"\
"				#endif\n"\
"			#endif\n"\
"		#elif defined(PER_PIXEL_LIGHTING)\n"\
"			//Lighting is calculated in world space\n"\
"			#ifdef DISPLACEMENTMAP\n"\
"				float parallaxHeight;\n"\
"				vec2 texcoord0 = ParallaxMapping(v_texcoord0, normalize(v_vertexToEyeInTangentSpace), parallaxHeight);\n"\
"			#else\n"\
"				vec2 texcoord0 = v_texcoord0;\n"\
"			#endif\n"\
"			#ifdef NORMALMAP\n"\
"				//The normals in the map are stored in tangent/texture space.\n"\
"				//For better explanation see http://ogldev.atspace.co.uk/www/tutorial26/tutorial26.html\n"\
"				//We need to transform them to world space:\n"\
"				mat3 TBN = mat3(v_tangent, v_bitangent, v_normal); //Generate a world space transformation matrix using the tangent-bitangent-normal.\n"\
"				vec3 bumpMapNormal = texture2D(u_texture1, texcoord0).xyz; // Sample the normal from the normal map \n"\
"			    bumpMapNormal = 2.0 * bumpMapNormal - vec3(1.0); //Normalize from 0..1 to -1..1\n"\
"			    vec3 normal = normalize(TBN * bumpMapNormal); //Transform the normal from tangent/texture space to world space using the TBN matrix\n"\
"			#else\n"\
"				vec3 normal = normalize(v_normal);\n"\
"			#endif\n"\
"    		vec3 vertexToEye = normalize(v_vertexToEye);\n"\
"    		vec4 totalLight = CalcFSTotalLight(vertexToEye, normal);\n"\
"    		#ifdef LIGHTMAP\n"\
"    			#ifdef DIFFUSEMAP\n"\
"    				gl_FragColor = v_color * totalLight * texture2D(u_texture2, v_texcoord1) * texture2D(u_texture0, texcoord0);\n"\
"    			#else\n"\
"    				gl_FragColor = v_color * totalLight * texture2D(u_texture2, v_texcoord1);\n"\
"    			#endif\n"\
"    		#else\n"\
"    			#ifdef DIFFUSEMAP\n"\
"		    		gl_FragColor = v_color * totalLight * texture2D(u_texture0, texcoord0);\n"\
"		    	#else\n"\
"		    		gl_FragColor = v_color * totalLight;\n"\
"		    	#endif\n"\
"		    #endif\n"\
"		#elif defined(LIGHTMAP) // lightmap without lighting\n"\
"			\n"\
"			#ifdef DIFFUSEMAP\n"\
"				gl_FragColor = v_color * texture2D(u_texture2, v_texcoord1) * texture2D(u_texture0, v_texcoord0);\n"\
"			#else\n"\
"				gl_FragColor = v_color * texture2D(u_texture2, v_texcoord1);\n"\
"			#endif\n"\
"		#else // Vertex color by default\n"\
"			gl_FragColor = v_color;\n"\
"		#endif	    \n"\
"	}	\n"\
"#endif\n"\
;
}
