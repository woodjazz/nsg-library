#include "gles2VertexCompatibility.h"
"\
struct LightSource\
{\
  vec4 position;\
  vec4 diffuse;\
  float constantAttenuation, linearAttenuation, quadraticAttenuation;\
  float spotCutoff, spotExponent;\
  vec3 spotDirection;\
};\
\
LightSource light0 = LightSource(\
    vec4(-1.0,  4.0,  4.0, 1.0),\
    vec4(1.0, 1.0, 1.0, 1.0),\
    0.0, 0.1, 0.0,\
    100.0, 20.0,\
    vec3(-1.0, -0.5, -1.0)\
);\
\
struct Material\
{\
  vec4 diffuse;\
};\
\
Material mymaterial = Material(vec4(1.0, 0.8, 0.8, 1.0));\
\
uniform mat4 u_m, u_mvp;\
uniform mat3 u_model_inv_transp;\
attribute vec2 a_texcoord;\
attribute vec4 a_position;\
attribute vec3 a_normal;\
\
varying vec4 v_color;\
varying vec2 v_texcoord;\
\
void main() \
{\
	vec3 normalDirection = normalize(u_model_inv_transp * a_normal);\n\
	vec3 lightDirection;\n\
	float attenuation;\n\
	if(light0.position.w == 0.0)\n\
	{\n\
		// directional light\n\
		attenuation = 1.0; // no attenuation\n\
		lightDirection = normalize(vec3(light0.position));\n\
	}\n\
	else\n\
	{\n\
		// point or spot light (or other kind of light)\n\
		vec3 vertexToLightSource = vec3(light0.position - u_m * a_position);\n\
		float distance = length(vertexToLightSource);\n\
		lightDirection = normalize(vertexToLightSource);\n\
		attenuation = 1.0 / (light0.constantAttenuation + light0.linearAttenuation * distance + light0.quadraticAttenuation * distance * distance);\n\
		if (light0.spotCutoff <= 90.0)\n\
		{\n\
			// spotlight\n\
			float clampedCosine = max(0.0, dot(-lightDirection, normalize(light0.spotDirection)));\n\
			if(clampedCosine < cos(light0.spotCutoff * 3.14159 / 180.0))\n\
			{\n\
				// outside of spotlight cone\n\
				attenuation = 0.0;\n\
			}\n\
			else\n\
			{\n\
				attenuation = attenuation * pow(clampedCosine, light0.spotExponent);\n\
			}\n\
		}\n\
	}\n\
	vec3 diffuseReflection = attenuation * vec3(light0.diffuse) * vec3(mymaterial.diffuse) * max(0.0, dot(normalDirection, lightDirection));\n\
	v_color = vec4(diffuseReflection, 1.0);\n\
	gl_Position = u_mvp * a_position;\n\
	v_texcoord = a_texcoord;\n\
}\n\
"

