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

#if defined(AMBIENT_PASS)

	uniform vec4 u_sceneAmbientColor;

	varying vec4 v_color;
	varying vec2 v_texcoord0;

	#if defined(AOMAP1) || defined(LIGHTMAP1)
		varying vec2 v_texcoord1;
	#endif

#elif defined(SHADOWCUBE_PASS) || defined(SHADOW_PASS)

	varying vec3 v_worldPos;

#else // LIT_PASS

	varying vec4 v_color;
	varying vec2 v_texcoord0;
	varying vec3 v_worldPos;
	varying vec3 v_normal;
	varying vec3 v_tangent;
	varying vec3 v_bitangent;
	varying vec2 v_texcoord1;
	varying vec3 v_vertexToEye;

	#if defined(SHADOWMAP)

		varying vec4 v_shadowClipPos;
		uniform float u_shadowMapInvSize;

	#elif defined(CUBESHADOWMAP)

		uniform float u_shadowMapInvSize;

	#endif

	struct BaseLight
	{
	    vec4 diffuse;
	    vec4 specular;
	};

	#if defined(HAS_DIRECTIONAL_LIGHT)

		uniform vec4 u_shadowColor;
		varying vec3 v_lightDirection;
		struct DirectionalLight
		{
		    BaseLight base;
		    vec3 direction;
		    vec3 position; // really is the shadow camera position
		};
		uniform DirectionalLight u_directionalLight;

	#elif defined(HAS_POINT_LIGHT)

		uniform vec4 u_shadowColor;
		varying vec3 v_lightDirection;
		struct PointLight
		{
		    BaseLight base;
		    vec3 position;
		};
		uniform PointLight u_pointLight;

	#elif defined(HAS_SPOT_LIGHT)

		uniform vec4 u_shadowColor;
		varying vec3 v_lightDirection;
		struct SpotLight
		{
		    BaseLight base;
		    vec3 position;
		    vec3 direction;
		    float cutOff; // 0.5f * cosine(cutOff)
		};
		uniform SpotLight u_spotLight;

	#endif

	uniform float u_shadowBias;

#endif

struct Material
{
	vec4 color;
    float ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
};

uniform Material u_material;
uniform vec3 u_eyeWorldPos;
uniform float u_lightInvRange;
