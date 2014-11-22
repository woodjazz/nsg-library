#pragma once
namespace NSG
{
static const char* POSTPROCESS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#ifdef BLEND\n"\
"	uniform int u_blendMode;\n"\
"	vec4 Blend()\n"\
"	{\n"\
"		vec4 dst = texture2D(u_texture0, v_texcoord0); // rendered scene\n"\
"		vec4 src = texture2D(u_texture1, v_texcoord0); // for example the glow map \n"\
"		if ( u_blendMode == 0 )\n"\
"		{\n"\
"			// Additive blending (strong result, high overexposure)\n"\
"			return min(src + dst, 1.0);\n"\
"		}\n"\
"		else if ( u_blendMode == 1 )\n"\
"		{\n"\
"			// Screen blending (mild result, medium overexposure)\n"\
"			vec4 color = clamp((src + dst) - (src * dst), 0.0, 1.0);\n"\
"			color.w = 1.0;\n"\
"			return color;\n"\
"		}\n"\
"		else if ( u_blendMode == 2 )\n"\
"		{\n"\
"			// Softlight blending (light result, no overexposure)\n"\
"			// Due to the nature of soft lighting, we need to bump the black region of the glowmap\n"\
"			// to 0.5, otherwise the blended result will be dark (black soft lighting will darken\n"\
"			// the image).\n"\
"			src = (src * 0.5) + 0.5;\n"\
"			float x = (src.x <= 0.5) ? (dst.x - (1.0 - 2.0 * src.x) * dst.x * (1.0 - dst.x)) : (((src.x > 0.5) && (dst.x <= 0.25)) ? (dst.x + (2.0 * src.x - 1.0) * (4.0 * dst.x * (4.0 * dst.x + 1.0) * (dst.x - 1.0) + 7.0 * dst.x)) : (dst.x + (2.0 * src.x - 1.0) * (sqrt(dst.x) - dst.x)));\n"\
"			float y = (src.y <= 0.5) ? (dst.y - (1.0 - 2.0 * src.y) * dst.y * (1.0 - dst.y)) : (((src.y > 0.5) && (dst.y <= 0.25)) ? (dst.y + (2.0 * src.y - 1.0) * (4.0 * dst.y * (4.0 * dst.y + 1.0) * (dst.y - 1.0) + 7.0 * dst.y)) : (dst.y + (2.0 * src.y - 1.0) * (sqrt(dst.y) - dst.y)));\n"\
"			float z = (src.z <= 0.5) ? (dst.z - (1.0 - 2.0 * src.z) * dst.z * (1.0 - dst.z)) : (((src.z > 0.5) && (dst.z <= 0.25)) ? (dst.z + (2.0 * src.z - 1.0) * (4.0 * dst.z * (4.0 * dst.z + 1.0) * (dst.z - 1.0) + 7.0 * dst.z)) : (dst.z + (2.0 * src.z - 1.0) * (sqrt(dst.z) - dst.z)));\n"\
"			\n"\
"			return vec4(x, y, z, 1.0);\n"\
"		}\n"\
"		else\n"\
"		{\n"\
"			return src;\n"\
"		}\n"\
"	}\n"\
"#elif defined(BLUR)\n"\
"	const int BLUR_KERNEL_HALF_SIZE = 2;\n"\
"	uniform vec2 u_blurDir;\n"\
"	uniform vec2 u_blurRadius;\n"\
"	uniform float u_sigma;\n"\
"	// Adapted: http://callumhay.blogspot.com/2010/09/gaussian-blur-shader-glsl.html\n"\
"	vec4 GaussianBlur(vec2 blurDir, vec2 blurRadius, float sigma, sampler2D texSampler, vec2 texCoord)\n"\
"	{\n"\
"		const float PI = 3.14159265;\n"\
"		// Incremental Gaussian Coefficent Calculation (See GPU Gems 3 pp. 877 - 889)\n"\
"	    vec3 gaussCoeff;\n"\
"	    gaussCoeff.x = 1.0 / (sqrt(2.0 * PI) * sigma);\n"\
"	    gaussCoeff.y = exp(-0.5 / (sigma * sigma));\n"\
"	    gaussCoeff.z = gaussCoeff.y * gaussCoeff.y;\n"\
"	    vec2 blurVec = blurRadius * blurDir;\n"\
"	    vec4 avgValue = vec4(0.0);\n"\
"	    float gaussCoeffSum = 0.0;\n"\
"	    avgValue += texture2D(texSampler, texCoord) * gaussCoeff.x;\n"\
"	    gaussCoeffSum += gaussCoeff.x;\n"\
"	    gaussCoeff.xy *= gaussCoeff.yz;\n"\
"	    for (int i = 1; i <= BLUR_KERNEL_HALF_SIZE; i++)\n"\
"	    {\n"\
"	        avgValue += texture2D(texSampler, texCoord - float(i) * blurVec) * gaussCoeff.x;\n"\
"	        avgValue += texture2D(texSampler, texCoord + float(i) * blurVec) * gaussCoeff.x;\n"\
"	        gaussCoeffSum += 2.0 * gaussCoeff.x;\n"\
"	        gaussCoeff.xy *= gaussCoeff.yz;\n"\
"	    }\n"\
"	    return avgValue / gaussCoeffSum;\n"\
"	}\n"\
"	vec4 Blur()\n"\
"	{\n"\
"		return GaussianBlur(u_blurDir, u_blurRadius, u_sigma, u_texture0, v_texcoord0);\n"\
"	}\n"\
"#endif\n"\
;
}
