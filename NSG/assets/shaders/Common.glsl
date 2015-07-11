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
};

uniform vec3 u_sceneAmbientColor;
uniform vec3 u_sceneHorizonColor;
uniform Material u_material;
// Could be normal camera or shadow camera
uniform vec3 u_eyeWorldPos;
// The Z far value in the shadow camera used to know the correct split
// one coordinate per split (in case of directional light)
uniform vec4 u_shadowCameraZFar;
uniform vec4 u_shadowColor;
uniform float u_shadowBias;
uniform vec4 u_shadowMapInvSize;

// The light position for each split
#define MAX_SPLITS 4
// Split 1 to 3 only applies to directional light and is the shadow camera's position
uniform vec3 u_lightPosition[MAX_SPLITS]; // (the default split) for directional light is the shadow camera's position
// one coordinate per split (in case of directional light)
uniform vec4 u_lightInvRange; 
uniform vec3 u_lightDirection;
uniform vec4 u_lightDiffuseColor;
uniform vec4 u_lightSpecularColor;
uniform float u_lightCutOff; // 0.5f * cosine(cutOff)
uniform mat4 u_view;
uniform float u_fogMinIntensity;
uniform float u_fogStart;
uniform float u_fogEnd;
uniform float u_fogHeight;

#ifdef COMPILEVS
	uniform mat4 u_model;
	uniform mat3 u_normalMatrix;
	uniform mat4 u_viewProjection;
	uniform mat4 u_projection;
	uniform vec4 u_uvTransform;
	#if defined(SKINNED)
		uniform mat4 u_bones[MAX_BONES];
	#endif
#else 
	//COMPILEFS
	uniform mat4 u_lightViewProjection[MAX_SPLITS];
#endif


