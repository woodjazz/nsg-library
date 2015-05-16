//Remember to rebuild with CMake if this file changes
#ifdef GLES2
	#ifdef GL_FRAGMENT_PRECISION_HIGH
		precision highp float;
		precision highp int;
	#else
		precision mediump float;
		precision mediump int;
	#endif
#else
	#define lowp
	#define mediump
	#define highp
#endif   

varying vec4 v_color;
varying vec2 v_texcoord0; 

#if defined(AMBIENT)

	#if defined(AOMAP1) || defined(LIGHTMAP1)
		varying vec2 v_texcoord1;
	#endif

#elif defined(HAS_DIRECTIONAL_LIGHT) || defined(HAS_POINT_LIGHT) || defined(HAS_SPOT_LIGHT)

	varying vec3 v_normal;
	varying vec3 v_tangent;
	varying vec3 v_bitangent;
	varying vec2 v_texcoord1;
	varying vec3 v_vertexToEye;

	struct Attenuation
	{
	    float constant;
	    float linear;
	    float quadratic;
	};

	struct BaseLight
	{
	    vec4 diffuse;
	    vec4 specular;
	};

	uniform vec3 u_eyeWorldPos;

	#if defined(HAS_DIRECTIONAL_LIGHT)

		struct DirectionalLight
		{
		    BaseLight base;
		    vec3 direction;
		};
		uniform DirectionalLight u_directionalLight;

	#elif defined(HAS_POINT_LIGHT)

		varying vec3 v_lightDirection;
		struct PointLight
		{
		    BaseLight base;
		    vec3 position;
		    Attenuation atten;
		};
		uniform PointLight u_pointLight;

	#elif defined(HAS_SPOT_LIGHT)

		varying vec3 v_light2Pixel;
		struct SpotLight
		{
		    BaseLight base;
		    vec3 position;
		    vec3 direction;
		    Attenuation atten;
		    float cutOff; // 0.5f * cosine(cutOff)
		};
		uniform SpotLight u_spotLight;

	#endif

#endif

struct Material
{
	vec4 color;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform Material u_material;
uniform vec4 u_sceneAmbientColor;
