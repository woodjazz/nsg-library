#pragma once
#include <string>
namespace NSG
{
static const std::string POSTPROCESS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#ifdef BLEND\n"\
"	uniform int u_blendMode;\n"\
"	vec4 Blend()\n"\
"	{\n"\
"		vec4 dst = texture2D(u_texture0, v_texcoord); // rendered scene\n"\
"		vec4 src = texture2D(u_texture1, v_texcoord); // for example the glow map \n"\
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
"	uniform vec2 u_texelSize;\n"\
"	uniform int u_orientation;\n"\
"	uniform int u_blurAmount;\n"\
"	uniform float u_blurScale;\n"\
"	uniform float u_blurStrength;\n"\
"	/// Gets the Gaussian value in the first dimension.\n"\
"	/// <param name=x>Distance from origin on the x-axis.</param>\n"\
"	/// <param name=deviation>Standard deviation.</param>\n"\
"	/// <returns>The gaussian value on the x-axis.</returns>\n"\
"	float Gaussian(float x, float deviation)\n"\
"	{\n"\
"		return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));	\n"\
"	}\n"\
"	vec4 Blur()\n"\
"	{\n"\
"		float halfBlur = float(u_blurAmount) * 0.5;\n"\
"		vec4 colour = vec4(0.0);\n"\
"		vec4 texColour = vec4(0.0);\n"\
"		\n"\
"		// Gaussian deviation\n"\
"		float deviation = halfBlur * 0.35;\n"\
"		deviation *= deviation;\n"\
"		float strength = 1.0 - u_blurStrength;\n"\
"		\n"\
"		//if ( u_orientation == 0 )\n"\
"		{\n"\
"			// Horizontal blur\n"\
"			for (int i = 0; i < 100; ++i)\n"\
"			{\n"\
"				if ( i >= u_blurAmount )\n"\
"					break;\n"\
"				\n"\
"				float offset = float(i) - halfBlur;\n"\
"				texColour = texture2D(u_texture0, v_texcoord + vec2(offset * u_texelSize.x * u_blurScale, 0.0)) * Gaussian(offset * strength, deviation);\n"\
"				colour += texColour;\n"\
"			}\n"\
"		}\n"\
"		//else\n"\
"		{\n"\
"			// Vertical blur\n"\
"			for (int i = 0; i < 100; ++i)\n"\
"			{\n"\
"				if ( i >= u_blurAmount )\n"\
"					break;\n"\
"				\n"\
"				float offset = float(i) - halfBlur;\n"\
"				texColour = texture2D(u_texture0, v_texcoord + vec2(0.0, offset * u_texelSize.y * u_blurScale)) * Gaussian(offset * strength, deviation);\n"\
"				colour += texColour;\n"\
"			}\n"\
"		}\n"\
"		\n"\
"		// Apply colour\n"\
"		vec4 color = clamp(colour, 0.0, 1.0);\n"\
"		color.w = 1.0;\n"\
"		return color;\n"\
"	}\n"\
"#endif\n"\
;
}
