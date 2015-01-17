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

#ifdef HAS_LIGHTS
	varying vec3 v_normal;
	varying vec3 v_tangent;
	varying vec3 v_bitangent;
	varying vec2 v_texcoord1;
	varying vec3 v_vertexToEye;
	varying vec3 v_vertexToEyeInTangentSpace;
	#ifdef HAS_POINT_LIGHTS
		varying vec3 v_lightDirection[NUM_POINT_LIGHTS];
	#endif
	#ifdef HAS_SPOT_LIGHTS
		varying vec3 v_light2Pixel[NUM_SPOT_LIGHTS];
	#endif
#endif

struct Material
{
	vec4 color;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    float parallaxScale;
};

uniform Material u_material;

