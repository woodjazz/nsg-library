struct LightSource
{
	vec4 position;
	vec4 diffuse;
	float constantAttenuation, linearAttenuation, quadraticAttenuation;
	float spotCutoff, spotExponent;
	vec3 spotDirection;
};

LightSource light0 = LightSource(
    vec4(-1.0,  4.0,  4.0, 1.0),
    vec4(1.0, 1.0, 1.0, 1.0),
    0.0, 0.1, 0.0,
    100.0, 20.0,
    vec3(-1.0, -0.5, -1.0)
);

struct Material
{
	vec4 diffuse;
};

uniform Material u_material;
uniform mat4 u_m, u_mvp;
uniform mat3 u_model_inv_transp;
attribute vec2 a_texcoord;
attribute vec4 a_position;
attribute vec3 a_normal;

varying vec4 v_color;
varying vec2 v_texcoord;

void main()
{
	vec3 normalDirection = normalize(u_model_inv_transp * a_normal);
	vec3 lightDirection;
	float attenuation;
	
	if(light0.position.w == 0.0)
	{
		// directional light
		attenuation = 1.0; // no attenuation
		lightDirection = normalize(vec3(light0.position));
	}
	else
	{
		// point or spot light (or other kind of light)
		vec3 vertexToLightSource = vec3(light0.position - u_m * a_position);
		float distance = length(vertexToLightSource);
		lightDirection = normalize(vertexToLightSource);
		attenuation = 1.0 / (light0.constantAttenuation + light0.linearAttenuation * distance + light0.quadraticAttenuation * distance * distance);

		if (light0.spotCutoff <= 90.0)
		{
			// spotlight
			float clampedCosine = max(0.0, dot(-lightDirection, normalize(light0.spotDirection)));
			
			if(clampedCosine < cos(light0.spotCutoff * 3.14159 / 180.0))
			{
				// outside of spotlight cone
				attenuation = 0.0;
			}
			else
			{
				attenuation = attenuation * pow(clampedCosine, light0.spotExponent);
			}
		}
	}
	vec3 diffuseReflection = attenuation * vec3(light0.diffuse) * vec3(u_material.diffuse) * max(0.0, dot(normalDirection, lightDirection));
	v_color = vec4(diffuseReflection, u_material.diffuse.w);
	gl_Position = u_mvp * a_position;
	v_texcoord = a_texcoord;
}


