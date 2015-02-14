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
"#endif   \n"\
"varying vec4 v_color;\n"\
"varying vec2 v_texcoord0; \n"\
"#ifdef HAS_LIGHTS\n"\
"	varying vec3 v_normal;\n"\
"	varying vec3 v_tangent;\n"\
"	varying vec3 v_bitangent;\n"\
"	varying vec2 v_texcoord1;\n"\
"	varying vec3 v_vertexToEye;\n"\
"	varying vec3 v_vertexToEyeInTangentSpace;\n"\
"	struct Attenuation\n"\
"	{\n"\
"	    float constant;\n"\
"	    float linear;\n"\
"	    float quadratic;\n"\
"	};\n"\
"	struct BaseLight\n"\
"	{\n"\
"	    vec4 ambient;\n"\
"	    vec4 diffuse;\n"\
"	    vec4 specular;\n"\
"	};\n"\
"	uniform vec4 u_sceneAmbientColor;\n"\
"	uniform vec3 u_eyeWorldPos;\n"\
"	#ifdef HAS_DIRECTIONAL_LIGHTS\n"\
"		struct DirectionalLight\n"\
"		{\n"\
"		    int enabled;\n"\
"		    BaseLight base;\n"\
"		    vec3 direction;\n"\
"		};\n"\
"		uniform DirectionalLight u_directionalLight[NUM_DIRECTIONAL_LIGHTS];\n"\
"	#endif\n"\
"	#ifdef HAS_POINT_LIGHTS\n"\
"		varying vec3 v_lightDirection[NUM_POINT_LIGHTS];\n"\
"		struct PointLight\n"\
"		{\n"\
"		    int enabled;\n"\
"		    BaseLight base;\n"\
"		    vec3 position;\n"\
"		    Attenuation atten;\n"\
"		};\n"\
"		uniform PointLight u_pointLights[NUM_POINT_LIGHTS];\n"\
"	#endif\n"\
"	#ifdef HAS_SPOT_LIGHTS\n"\
"		varying vec3 v_light2Pixel[NUM_SPOT_LIGHTS];\n"\
"		struct SpotLight\n"\
"		{\n"\
"		    int enabled;\n"\
"		    BaseLight base;\n"\
"		    vec3 position;\n"\
"		    vec3 direction;\n"\
"		    Attenuation atten;\n"\
"		    float cutOff; // 0.5f * cosine(cutOff)\n"\
"		};\n"\
"		uniform SpotLight u_spotLights[NUM_SPOT_LIGHTS];\n"\
"	#endif\n"\
"#elif defined(LIGHTMAP)\n"\
"	varying vec2 v_texcoord1;\n"\
"#endif\n"\
"struct Material\n"\
"{\n"\
"	vec4 color;\n"\
"#ifdef HAS_LIGHTS	\n"\
"    vec4 ambient;\n"\
"    vec4 diffuse;\n"\
"    vec4 specular;\n"\
"    float shininess;\n"\
"    float parallaxScale;\n"\
"#endif    \n"\
"};\n"\
"uniform Material u_material;\n"\
;
}
