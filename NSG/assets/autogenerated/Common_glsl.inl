#pragma once
namespace NSG
{
static const char* COMMON_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#ifdef GLES2\n"\
"	#ifdef GL_FRAGMENT_PRECISION_HIGH\n"\
"		precision highp float;\n"\
"		precision highp int;\n"\
"	#else\n"\
"		precision mediump float;\n"\
"		precision mediump int;\n"\
"	#endif\n"\
"#else\n"\
"	#define lowp\n"\
"	#define mediump\n"\
"	#define highp\n"\
"#endif\n"\
"struct Material\n"\
"{\n"\
"	vec4 color;\n"\
"	vec4 ambient;\n"\
"	vec4 diffuse;\n"\
"	vec4 specular;\n"\
"	float shininess;\n"\
"};\n"\
"struct BaseLight                                                                    \n"\
"{                \n"\
"	vec4 diffuse;\n"\
"	vec4 specular;\n"\
"};\n"\
"struct DirectionalLight\n"\
"{\n"\
"    BaseLight base;\n"\
"    vec3 direction;\n"\
"};\n"\
"struct Attenuation\n"\
"{\n"\
"    float constant;\n"\
"    float linear;\n"\
"    float quadratic;\n"\
"};                                                                \n"\
"                                                                                    \n"\
"struct PointLight                                                                           \n"\
"{                                                                                           \n"\
"    BaseLight base;                                                                         \n"\
"    vec3 position;                                                                          \n"\
"    Attenuation atten;                                                                      \n"\
"};                                                                                          \n"\
"uniform mat4 u_model;\n"\
"uniform mat3 u_normalMatrix;\n"\
"uniform mat4 u_viewProjection; \n"\
"uniform vec4 u_sceneAmbientColor;\n"\
"uniform vec3 u_eyeWorldPos;\n"\
"uniform Material u_material;\n"\
"uniform int u_numPointLights;\n"\
"uniform DirectionalLight u_directionalLight;                                                 \n"\
"uniform PointLight u_pointLights[MAX_POINT_LIGHTS];\n"\
"#ifdef COMPILEVS\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	// Vertex shader specific\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	attribute vec4 a_position;\n"\
"	attribute vec2 a_texcoord;\n"\
"	attribute vec3 a_normal;\n"\
"	attribute vec4 a_color;\n"\
"	#if defined(INSTANCED)\n"\
"	    attribute vec4 a_mMatrixRow0;\n"\
"	    attribute vec4 a_mMatrixRow1;\n"\
"	    attribute vec4 a_mMatrixRow2;\n"\
"	    attribute vec3 a_normalMatrixCol0;\n"\
"	    attribute vec3 a_normalMatrixCol1;\n"\
"	    attribute vec3 a_normalMatrixCol2;\n"\
"	#endif\n"\
"	varying vec4 v_color;\n"\
"	varying vec3 v_normal;\n"\
"	varying vec2 v_texcoord;\n"\
"	\n"\
"	mat4 GetModelMatrix()\n"\
"	{\n"\
"		#if defined(INSTANCED)\n"\
"			// Since we are using rows instead of cols the instancing model matrix is a transpose, \n"\
"			// so the matrix multiply order must be swapped\n"\
"			const vec4 lastColumn = vec4(0.0, 0.0, 0.0, 1.0);\n"\
"		    return mat4(a_mMatrixRow0, a_mMatrixRow1, a_mMatrixRow2, lastColumn);\n"\
"	    #else\n"\
"		    return u_model;\n"\
"	    #endif\n"\
"	}\n"\
"	mat3 GetNormalMatrix()\n"\
"	{\n"\
"		#if defined(INSTANCED)\n"\
"			return mat3(a_normalMatrixCol0, a_normalMatrixCol1, a_normalMatrixCol2);\n"\
"		#else\n"\
"			return u_normalMatrix;\n"\
"		#endif\n"\
"	}\n"\
"	vec3 GetWorldPos()\n"\
"	{\n"\
"		#if defined(INSTANCED)\n"\
"			// Instancing model matrix is a transpose, so the matrix multiply order must be swapped\n"\
"			return (a_position * GetModelMatrix()).xyz;\n"\
"		#else\n"\
"	    	return (GetModelMatrix() * a_position).xyz;\n"\
"	    #endif\n"\
"	}\n"\
"	vec3 GetWorldNormal()\n"\
"	{\n"\
"		return normalize((GetNormalMatrix() * a_normal));\n"\
"	}\n"\
"	vec4 GetClipPos(vec3 worldPos)\n"\
"	{\n"\
"	    return u_viewProjection * vec4(worldPos, 1.0);\n"\
"	}\n"\
"	vec4 CalcLight(vec3 worldPos, BaseLight light, vec3 lightDirection, vec3 normal)                   \n"\
"	{                                                                                           \n"\
"	    vec4 color = u_sceneAmbientColor * u_material.ambient;\n"\
"		\n"\
"		float diffuseFactor = dot(normal, -lightDirection);	\n"\
"	    if (diffuseFactor > 0.0) \n"\
"	    {                                                                \n"\
"	        color = max(color, light.diffuse * u_material.diffuse);    \n"\
"	        #ifdef SPECULAR\n"\
"		        vec3 vertexToEye = normalize(u_eyeWorldPos - worldPos);                             \n"\
"		        vec3 lightReflect = normalize(reflect(lightDirection, normal));\n"\
"		        float specularFactor = dot(vertexToEye, lightReflect);\n"\
"		        specularFactor = pow(specularFactor, u_material.shininess);\n"\
"		        if (specularFactor > 0.0)\n"\
"		        {\n"\
"		            color += light.specular * u_material.specular * specularFactor;\n"\
"		       	}\n"\
"		    #endif\n"\
"	    }                                                                                       \n"\
"	                                                                                            \n"\
"	    return color;  \n"\
"	}                                                                                           \n"\
"	vec4 CalcDirectionalLight(vec3 worldPos, vec3 normal)\n"\
"	{                                                                                           \n"\
"	    return CalcLight(worldPos, u_directionalLight.base, u_directionalLight.direction, normal);\n"\
"	}                                                      \n"\
"	vec4 CalcPointLight(vec3 worldPos, int index, vec3 normal)\n"\
"	{                                                                                           \n"\
"	    vec3 lightDirection = worldPos - u_pointLights[index].position;                         \n"\
"	    float distance = length(lightDirection);                                                \n"\
"	    lightDirection = normalize(lightDirection);                                             \n"\
"	                                                                                            \n"\
"	    vec4 color = CalcLight(worldPos, u_pointLights[index].base, lightDirection, normal);       \n"\
"	    float attenuation =  u_pointLights[index].atten.constant +                               \n"\
"	                         u_pointLights[index].atten.linear * distance +                      \n"\
"	                         u_pointLights[index].atten.quadratic * distance * distance;               \n"\
"	                                                                                            \n"\
"	    return color / attenuation;                                                             \n"\
"	}\n"\
"	#if !defined(HAS_USER_VERTEX_SHADER)\n"\
"	void main()\n"\
"	{\n"\
"		#ifdef SHOW_TEXTURE\n"\
"			gl_Position = a_position;\n"\
"			v_texcoord = vec2(a_texcoord.x, 1.0 - a_texcoord.y);\n"\
"		#elif defined(BLUR) || defined(BLEND)\n"\
"			gl_Position = a_position;\n"\
"			v_texcoord = a_texcoord;\n"\
"		#elif defined(STENCIL)\n"\
"			gl_Position = GetClipPos(GetWorldPos());	\n"\
"		#else\n"\
"			vec3 worldPos = GetWorldPos();\n"\
"			#ifdef DIFFUSE\n"\
"			    vec3 normal = GetWorldNormal();\n"\
"			    vec4 totalLight = CalcDirectionalLight(worldPos, normal);\n"\
"			    for (int i = 0 ; i < u_numPointLights ; i++) \n"\
"			    {\n"\
"			        totalLight += CalcPointLight(worldPos, i, normal);                                            \n"\
"			    }                                                                                       \n"\
"			    v_color = a_color * totalLight;\n"\
"			#else\n"\
"				v_color = u_material.color * a_color;\n"\
"			#endif\n"\
"		    v_texcoord = a_texcoord;\n"\
"			gl_Position = GetClipPos(worldPos);\n"\
"		#endif\n"\
"	}\n"\
"	#endif	\n"\
"#else\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	// Fragment shader specific\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	/////////////////////////////////////////////////////////////////\n"\
"	uniform sampler2D u_texture0;\n"\
"	uniform sampler2D u_texture1;\n"\
"	varying vec4 v_color;\n"\
"	varying vec3 v_normal;\n"\
"	varying vec2 v_texcoord;\n"\
"	#ifdef BLEND\n"\
"		uniform int u_blendMode;\n"\
"		vec4 Blend()\n"\
"		{\n"\
"			vec4 dst = texture2D(u_texture0, v_texcoord); // rendered scene\n"\
"			vec4 src = texture2D(u_texture1, v_texcoord); // for example the glow map \n"\
"			if ( u_blendMode == 0 )\n"\
"			{\n"\
"				// Additive blending (strong result, high overexposure)\n"\
"				return min(src + dst, 1.0);\n"\
"			}\n"\
"			else if ( u_blendMode == 1 )\n"\
"			{\n"\
"				// Screen blending (mild result, medium overexposure)\n"\
"				vec4 color = clamp((src + dst) - (src * dst), 0.0, 1.0);\n"\
"				color.w = 1.0;\n"\
"				return color;\n"\
"			}\n"\
"			else if ( u_blendMode == 2 )\n"\
"			{\n"\
"				// Softlight blending (light result, no overexposure)\n"\
"				// Due to the nature of soft lighting, we need to bump the black region of the glowmap\n"\
"				// to 0.5, otherwise the blended result will be dark (black soft lighting will darken\n"\
"				// the image).\n"\
"				src = (src * 0.5) + 0.5;\n"\
"				float x = (src.x <= 0.5) ? (dst.x - (1.0 - 2.0 * src.x) * dst.x * (1.0 - dst.x)) : (((src.x > 0.5) && (dst.x <= 0.25)) ? (dst.x + (2.0 * src.x - 1.0) * (4.0 * dst.x * (4.0 * dst.x + 1.0) * (dst.x - 1.0) + 7.0 * dst.x)) : (dst.x + (2.0 * src.x - 1.0) * (sqrt(dst.x) - dst.x)));\n"\
"				float y = (src.y <= 0.5) ? (dst.y - (1.0 - 2.0 * src.y) * dst.y * (1.0 - dst.y)) : (((src.y > 0.5) && (dst.y <= 0.25)) ? (dst.y + (2.0 * src.y - 1.0) * (4.0 * dst.y * (4.0 * dst.y + 1.0) * (dst.y - 1.0) + 7.0 * dst.y)) : (dst.y + (2.0 * src.y - 1.0) * (sqrt(dst.y) - dst.y)));\n"\
"				float z = (src.z <= 0.5) ? (dst.z - (1.0 - 2.0 * src.z) * dst.z * (1.0 - dst.z)) : (((src.z > 0.5) && (dst.z <= 0.25)) ? (dst.z + (2.0 * src.z - 1.0) * (4.0 * dst.z * (4.0 * dst.z + 1.0) * (dst.z - 1.0) + 7.0 * dst.z)) : (dst.z + (2.0 * src.z - 1.0) * (sqrt(dst.z) - dst.z)));\n"\
"				\n"\
"				return vec4(x, y, z, 1.0);\n"\
"			}\n"\
"			else\n"\
"			{\n"\
"				return src;\n"\
"			}\n"\
"		}\n"\
"	#elif defined(BLUR)\n"\
"		uniform vec2 u_texelSize;\n"\
"		uniform int u_orientation;\n"\
"		uniform int u_blurAmount;\n"\
"		uniform float u_blurScale;\n"\
"		uniform float u_blurStrength;\n"\
"		/// Gets the Gaussian value in the first dimension.\n"\
"		/// <param name=x>Distance from origin on the x-axis.</param>\n"\
"		/// <param name=deviation>Standard deviation.</param>\n"\
"		/// <returns>The gaussian value on the x-axis.</returns>\n"\
"		float Gaussian(float x, float deviation)\n"\
"		{\n"\
"			return (1.0 / sqrt(2.0 * 3.141592 * deviation)) * exp(-((x * x) / (2.0 * deviation)));	\n"\
"		}\n"\
"		vec4 Blur()\n"\
"		{\n"\
"			float halfBlur = float(u_blurAmount) * 0.5;\n"\
"			vec4 colour = vec4(0.0);\n"\
"			vec4 texColour = vec4(0.0);\n"\
"			\n"\
"			// Gaussian deviation\n"\
"			float deviation = halfBlur * 0.35;\n"\
"			deviation *= deviation;\n"\
"			float strength = 1.0 - u_blurStrength;\n"\
"			\n"\
"			//if ( u_orientation == 0 )\n"\
"			{\n"\
"				// Horizontal blur\n"\
"				for (int i = 0; i < 100; ++i)\n"\
"				{\n"\
"					if ( i >= u_blurAmount )\n"\
"						break;\n"\
"					\n"\
"					float offset = float(i) - halfBlur;\n"\
"					texColour = texture2D(u_texture0, v_texcoord + vec2(offset * u_texelSize.x * u_blurScale, 0.0)) * Gaussian(offset * strength, deviation);\n"\
"					colour += texColour;\n"\
"				}\n"\
"			}\n"\
"			//else\n"\
"			{\n"\
"				// Vertical blur\n"\
"				for (int i = 0; i < 100; ++i)\n"\
"				{\n"\
"					if ( i >= u_blurAmount )\n"\
"						break;\n"\
"					\n"\
"					float offset = float(i) - halfBlur;\n"\
"					texColour = texture2D(u_texture0, v_texcoord + vec2(0.0, offset * u_texelSize.y * u_blurScale)) * Gaussian(offset * strength, deviation);\n"\
"					colour += texColour;\n"\
"				}\n"\
"			}\n"\
"			\n"\
"			// Apply colour\n"\
"			vec4 color = clamp(colour, 0.0, 1.0);\n"\
"			color.w = 1.0;\n"\
"			return color;\n"\
"		}\n"\
"	#endif\n"\
"	#if !defined(HAS_USER_FRAGMENT_SHADER)\n"\
"		void main()\n"\
"		{\n"\
"			#if defined(TEXT)\n"\
"				gl_FragColor = v_color * vec4(1.0, 1.0, 1.0, texture2D(u_texture0, v_texcoord).a);\n"\
"			#elif defined(BLEND)\n"\
"				gl_FragColor = Blend();\n"\
"			#elif defined(BLUR)\n"\
"				gl_FragColor = Blur();\n"\
"			#elif defined(SHOW_TEXTURE)\n"\
"				gl_FragColor = texture2D(u_texture0, v_texcoord);\n"\
"			#elif defined(STENCIL)\n"\
"				gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"\
"			#else\n"\
"				gl_FragColor = v_color * texture2D(u_texture0, v_texcoord);\n"\
"			#endif	    \n"\
"		}	\n"\
"	#endif	\n"\
"#endif\n"\
;
}
