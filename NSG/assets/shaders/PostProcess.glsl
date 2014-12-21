//Remember to rebuild with CMake if this file changes

#ifdef BLEND

	uniform int u_blendMode;
	vec4 Blend()
	{
		vec4 dst = texture2D(u_texture0, v_texcoord0); // rendered scene
		vec4 src = texture2D(u_texture1, v_texcoord0); // for example the glow map 

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

	const int BLUR_KERNEL_HALF_SIZE = 2;
	uniform vec2 u_blurDir;
	uniform vec2 u_blurRadius;
	uniform float u_sigma;

	// Adapted: http://callumhay.blogspot.com/2010/09/gaussian-blur-shader-glsl.html
	vec4 GaussianBlur(vec2 blurDir, vec2 blurRadius, float sigma, sampler2D texSampler, vec2 texCoord)
	{
		const float PI = 3.14159265;

		// Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)
	    vec3 gaussCoeff;
	    gaussCoeff.x = 1.0 / (sqrt(2.0 * PI) * sigma);
	    gaussCoeff.y = exp(-0.5 / (sigma * sigma));
	    gaussCoeff.z = gaussCoeff.y * gaussCoeff.y;

	    vec2 blurVec = blurRadius * blurDir;
	    vec4 avgValue = vec4(0.0);
	    float gaussCoeffSum = 0.0;

	    avgValue += texture2D(texSampler, texCoord) * gaussCoeff.x;
	    gaussCoeffSum += gaussCoeff.x;
	    gaussCoeff.xy *= gaussCoeff.yz;

	    for (int i = 1; i <= BLUR_KERNEL_HALF_SIZE; i++)
	    {
	        avgValue += texture2D(texSampler, texCoord - float(i) * blurVec) * gaussCoeff.x;
	        avgValue += texture2D(texSampler, texCoord + float(i) * blurVec) * gaussCoeff.x;

	        gaussCoeffSum += 2.0 * gaussCoeff.x;
	        gaussCoeff.xy *= gaussCoeff.yz;
	    }

	    return avgValue / gaussCoeffSum;
	}

	vec4 Blur()
	{
		return GaussianBlur(u_blurDir, u_blurRadius, u_sigma, u_texture0, v_texcoord0);
	}

#endif