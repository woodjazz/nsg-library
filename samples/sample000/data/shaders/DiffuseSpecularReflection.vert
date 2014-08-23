
void main()
{
	vec3 normalDirection = normalize(u_model_inv_transp * a_normal);

	vec3 viewDirection = normalize(vec3(u_v_inv * vec4(0.0, 0.0, 0.0, 1.0) - u_m * a_position));

	vec3 lightDirection;
	
	float attenuation;
	
	if(u_light0.type == DIRECTIONAL_LIGHT)
	{
		// directional light
		attenuation = 1.0; // no attenuation
		lightDirection = normalize(u_light0.position);
	}
	else
	{
		// point or spot light (or other kind of light)
		vec3 vertexToLightSource = u_light0.position - vec3(u_m * a_position);
		float distance = length(vertexToLightSource);
		lightDirection = normalize(vertexToLightSource);
		attenuation = 1.0 / (u_light0.constantAttenuation + u_light0.linearAttenuation * distance + u_light0.quadraticAttenuation * distance * distance);

		if (u_light0.type == SPOT_LIGHT)
		{
			// spotlight
			float clampedCosine = max(0.0, dot(-lightDirection, normalize(u_light0.spotDirection)));
			
			if(clampedCosine < cos(u_light0.spotCutoff * 3.14159 / 180.0))
			{
				// outside of spotlight cone
				attenuation = 0.0;
			}
			else
			{
				attenuation = attenuation * pow(clampedCosine, u_light0.spotExponent);
			}
		}
	}
	
	vec3 ambientLighting = vec3(u_scene_ambient) * vec3(u_material.ambient);

	vec3 diffuseReflection = attenuation * vec3(u_light0.diffuse) * vec3(u_material.diffuse) * max(0.0, dot(normalDirection, lightDirection));

	vec3 specularReflection;
	if (dot(normalDirection, lightDirection) < 0.0) 
	{
		// light source on the wrong side => no specular reflection
		specularReflection = vec3(0.0, 0.0, 0.0);
	}
	else 
	{
		// light source on the right side
		specularReflection = attenuation * vec3(u_light0.specular) * vec3(u_material.specular) 
		* pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), u_material.shininess);
	}	

	v_color = vec4(ambientLighting + diffuseReflection + specularReflection, u_material.diffuse.w);
	gl_Position = u_mvp * a_position;
	v_texcoord = a_texcoord;
}


