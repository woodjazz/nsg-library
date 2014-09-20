//Remember to rebuild with CMake if this file changes
#ifdef BLEND

	uniform int u_blendMode;
	vec4 Blend()
	{
		vec4 dst = texture2D(u_texture0, v_texcoord); // rendered scene
		vec4 src = texture2D(u_texture1, v_texcoord); // for example the glow map 

		if ( u_blendMode == 0 )
		{
			// Additive blending (strong result, high overexposure)
			return min(src + dst, 1.0);
		}
		else if ( u_blendMode == 1 )
		{
			// Screen blending (mild result, medium overexposure)
			vec4 color = clamp((src + dst) - (src * dst), 0.0, 1.0);
			color.w = 1.0;
			return color;
		}
		else if ( u_blendMode == 2 )
		{
			// Softlight blending (light result, no overexposure)
			// Due to the nature of soft lighting, we need to bump the black region of the glowmap
			// to 0.5, otherwise the blended result will be dark (black soft lighting will darken
			// the image).
			src = (src * 0.5) + 0.5;

			float x = (src.x <= 0.5) ? (dst.x - (1.0 - 2.0 * src.x) * dst.x * (1.0 - dst.x)) : (((src.x > 0.5) && (dst.x <= 0.25)) ? (dst.x + (2.0 * src.x - 1.0) * (4.0 * dst.x * (4.0 * dst.x + 1.0) * (dst.x - 1.0) + 7.0 * dst.x)) : (dst.x + (2.0 * src.x - 1.0) * (sqrt(dst.x) - dst.x)));
			float y = (src.y <= 0.5) ? (dst.y - (1.0 - 2.0 * src.y) * dst.y * (1.0 - dst.y)) : (((src.y > 0.5) && (dst.y <= 0.25)) ? (dst.y + (2.0 * src.y - 1.0) * (4.0 * dst.y * (4.0 * dst.y + 1.0) * (dst.y - 1.0) + 7.0 * dst.y)) : (dst.y + (2.0 * src.y - 1.0) * (sqrt(dst.y) - dst.y)));
			float z = (src.z <= 0.5) ? (dst.z - (1.0 - 2.0 * src.z) * dst.z * (1.0 - dst.z)) : (((src.z > 0.5) && (dst.z <= 0.25)) ? (dst.z + (2.0 * src.z - 1.0) * (4.0 * dst.z * (4.0 * dst.z + 1.0) * (dst.z - 1.0) + 7.0 * dst.z)) : (dst.z + (2.0 * src.z - 1.0) * (sqrt(dst.z) - dst.z)));
			
			return vec4(x, y, z, 1.0);
		}
		else
		{
			return src;
		}
	}

#elif defined(BLUR)

	uniform vec2 u_texelSize;
	uniform int u_orientation;
	uniform int u_blurAmount;
	uniform float u_blurScale;
	uniform float u_blurStrength;

	/// Gets the Gaussian value in the first dimension.
	/// <param name=x>Distance from origin on the x-axis.</param>
	/// <param name=deviation>Standard deviation.</param>
	/// <returns>The gaussian value on the x-axis.</returns>
	float Gaussian(float x, float deviation)
	{
		return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));	
	}

	vec4 Blur()
	{
		float halfBlur = float(u_blurAmount) * 0.5;
		vec4 colour = vec4(0.0);
		vec4 texColour = vec4(0.0);
		
		// Gaussian deviation
		float deviation = halfBlur * 0.35;
		deviation *= deviation;
		float strength = 1.0 - u_blurStrength;
		
		//if ( u_orientation == 0 )
		{
			// Horizontal blur
			for (int i = 0; i < 100; ++i)
			{
				if ( i >= u_blurAmount )
					break;
				
				float offset = float(i) - halfBlur;
				texColour = texture2D(u_texture0, v_texcoord + vec2(offset * u_texelSize.x * u_blurScale, 0.0)) * Gaussian(offset * strength, deviation);
				colour += texColour;
			}
		}
		//else
		{
			// Vertical blur
			for (int i = 0; i < 100; ++i)
			{
				if ( i >= u_blurAmount )
					break;
				
				float offset = float(i) - halfBlur;
				texColour = texture2D(u_texture0, v_texcoord + vec2(0.0, offset * u_texelSize.y * u_blurScale)) * Gaussian(offset * strength, deviation);
				colour += texColour;
			}
		}
		
		// Apply colour
		vec4 color = clamp(colour, 0.0, 1.0);
		color.w = 1.0;
		return color;
	}

#endif