//Remember to rebuild with CMake if this file changes
#if defined(GLES2) || defined(EGL)
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
varying vec2 v_texcoord1;
varying vec3 v_worldPos;
varying vec3 v_normal;
varying vec3 v_tangent;
varying vec3 v_bitangent;
varying vec3 v_vertexToEye;
varying float v_depth;

struct Material
{
	vec4 diffuseColor;
	float diffuseIntensity;
	vec4 specularColor;
	float specularIntensity;
    float ambientIntensity;
    float shininess;
    float emitIntensity;
};

uniform vec3 u_sceneAmbientColor;
uniform vec3 u_sceneHorizonColor;
uniform Material u_material;
uniform vec3 u_eyeWorldPos; // camera or shadowCamera world position
// The Z far value in the shadow camera used to know the correct split
// one coordinate per split (in case of directional light)
uniform vec4 u_shadowCameraZFar;
uniform vec4 u_shadowColor;
//uniform float u_shadowBias; // constant bias
uniform vec4 u_shadowMapInvSize;

uniform vec3 u_lightPosition;
uniform float u_lightInvRange; // only used for point and spot lights
uniform vec3 u_lightDirection;
uniform vec4 u_lightDiffuseColor;
uniform vec4 u_lightSpecularColor;
uniform float u_lightCutOff; // 0.5f * cosine(cutOff)
uniform mat4 u_view; // could be main camera or shadowCamera
uniform float u_fogMinIntensity;
uniform float u_fogStart;
uniform float u_fogEnd;
uniform float u_fogHeight;

#ifdef COMPILEVS
	uniform mat4 u_model;
	uniform mat3 u_normalMatrix;
	uniform mat4 u_viewProjection; // could be main camera or shadowCamera
	uniform mat4 u_projection; // could be main camera or shadowCamera
	uniform vec4 u_uvTransform0; 
	uniform vec4 u_uvTransform1; 
	#if defined(SKINNED)
		uniform mat4 u_bones[MAX_BONES];
	#endif
#else //COMPILEFS
	
	#if defined(SPLITS1)
		#define MAX_SPLITS 1
	#elif defined(SPLITS2)
		#define MAX_SPLITS 2
	#elif defined(SPLITS3)
		#define MAX_SPLITS 3
	#elif defined(SPLITS4)
		#define MAX_SPLITS 5
	#endif

	#ifdef MAX_SPLITS		
		uniform mat4 u_lightView[MAX_SPLITS];
		uniform mat4 u_lightProjection[MAX_SPLITS];
		uniform mat4 u_lightViewProjection[MAX_SPLITS];
	#endif
#endif


