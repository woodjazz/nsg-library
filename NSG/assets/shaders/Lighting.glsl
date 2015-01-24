//Remember to rebuild with CMake if this file changes
#ifdef HAS_LIGHTS

struct BaseLight
{
    vec4 ambient;      
	vec4 diffuse;
	vec4 specular;
};

struct DirectionalLight
{
    int enabled;
    BaseLight base;
    vec3 direction;
};

struct Attenuation
{
    float constant;
    float linear;
    float quadratic;
};                                                                
                                                                                    
struct PointLight                                                                      
{
    int enabled;
    BaseLight base;
    vec3 position;
    Attenuation atten;
};

struct SpotLight                                                                      
{
    int enabled;                                                                          
    BaseLight base;
    vec3 position;
    vec3 direction;
    Attenuation atten;
    float cutOff; // 0.5f * cosine(cutOff)
};

uniform vec4 u_sceneAmbientColor;
uniform vec3 u_eyeWorldPos;

#ifdef HAS_DIRECTIONAL_LIGHTS
    uniform DirectionalLight u_directionalLight[NUM_DIRECTIONAL_LIGHTS];
#endif

#ifdef HAS_POINT_LIGHTS    
    uniform PointLight u_pointLights[NUM_POINT_LIGHTS];
#endif

#ifdef HAS_SPOT_LIGHTS    
    uniform SpotLight u_spotLights[NUM_SPOT_LIGHTS];
#endif    

vec4 CalcLight(BaseLight base, vec3 vertexToEye, vec3 lightDirection, vec3 normal)                   
{                               
    #ifdef AOMAP
        vec4 color = texture2D(u_texture4, v_texcoord0);
    #else                                                            
        vec4 color = u_material.ambient * base.ambient;
    #endif
	
	float diffuseFactor = dot(normal, -lightDirection);	

    if (diffuseFactor > 0.0) 
    {                                                                
        color = max(color, diffuseFactor * base.diffuse * u_material.diffuse);                                
        vec3 lightReflect = normalize(reflect(lightDirection, normal));
        float specularFactor = dot(vertexToEye, lightReflect);
    	specularFactor = pow(specularFactor, u_material.shininess);
        #ifdef SPECULARMAP
            color += specularFactor * base.specular * u_material.specular * texture2D(u_texture3, v_texcoord0);
        #else
            color += specularFactor * base.specular * u_material.specular;
        #endif
    }                                                                                       
                                                                            
    return color;  
}

vec4 CalcDirectionalLight(BaseLight base, vec3 lightDirection, vec3 vertexToEye, vec3 normal)
{                                                                             
    return CalcLight(base, vertexToEye, lightDirection, normal);
}

vec4 CalcPointLight(BaseLight base, vec3 lightDirection, Attenuation attenuation, vec3 vertexToEye, vec3 normal)
{                                                                                          
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);
    vec4 color = CalcLight(base, vertexToEye, lightDirection, normal);       
    float attenuationFactor =  attenuation.constant + attenuation.linear * distance + attenuation.quadratic * distance * distance;
    return color / attenuationFactor;
}

vec4 CalcSpotLight(BaseLight base, vec3 light2Pixel, vec3 lightDirection, Attenuation attenuation, vec3 vertexToEye, vec3 normal, float cutOff)
{
    lightDirection = normalize(lightDirection);
    light2Pixel = normalize(light2Pixel);
    float spotFactor = dot(light2Pixel, lightDirection);

    if(spotFactor > cutOff)
    {
        vec4 color = CalcPointLight(base, light2Pixel, attenuation, vertexToEye, normal);
        return color * (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - cutOff));
    }
    else
    {
        return vec4(0.0);
    }
}

vec4 CalcFSTotalLight(vec3 vertexToEye, vec3 normal)
{
    vec4 totalLight = u_sceneAmbientColor;

    #ifdef HAS_DIRECTIONAL_LIGHTS

        for (int i = 0 ; i < NUM_DIRECTIONAL_LIGHTS ; i++) 
        {
            if(u_directionalLight[i].enabled != 0)
                totalLight += CalcDirectionalLight(u_directionalLight[i].base, u_directionalLight[i].direction, vertexToEye, normal);
        }

    #endif

    #ifdef HAS_POINT_LIGHTS

        for (int i = 0 ; i < NUM_POINT_LIGHTS ; i++) 
        {
            if(u_pointLights[i].enabled != 0)
                totalLight += CalcPointLight(u_pointLights[i].base, v_lightDirection[i], u_pointLights[i].atten, vertexToEye, normal);                                            
        }

    #endif

    #ifdef HAS_SPOT_LIGHTS

        for (int i = 0 ; i < NUM_SPOT_LIGHTS ; i++) 
        {
            if(u_spotLights[i].enabled != 0)
                totalLight += CalcSpotLight(u_spotLights[i].base, v_light2Pixel[i], u_spotLights[i].direction, u_spotLights[i].atten, vertexToEye, normal, u_spotLights[i].cutOff); 
        }

    #endif

    return totalLight;
}

vec4 CalcVSTotalLight(vec3 worldPos, vec3 vertexToEye, vec3 normal)
{
    vec4 totalLight = u_sceneAmbientColor;

    #ifdef HAS_DIRECTIONAL_LIGHTS

        for (int i = 0 ; i < NUM_DIRECTIONAL_LIGHTS ; i++)
        {
            if(u_directionalLight[i].enabled != 0)
                totalLight += CalcDirectionalLight(u_directionalLight[i].base, u_directionalLight[i].direction, vertexToEye, normal);
        }

    #endif

    #ifdef HAS_POINT_LIGHTS

        for (int i = 0 ; i < NUM_POINT_LIGHTS ; i++) 
        {
            if(u_pointLights[i].enabled != 0)
                totalLight += CalcPointLight(u_pointLights[i].base, worldPos - u_pointLights[i].position, u_pointLights[i].atten, vertexToEye, normal);
        }

    #endif

    #ifdef HAS_SPOT_LIGHTS

        for (int i = 0 ; i < NUM_SPOT_LIGHTS ; i++) 
        {
            if(u_spotLights[i].enabled != 0)
                totalLight += CalcSpotLight(u_spotLights[i].base, worldPos - u_spotLights[i].position, u_spotLights[i].direction, u_spotLights[i].atten, vertexToEye, normal, u_spotLights[i].cutOff); 
        }

    #endif

    return totalLight;
}
#endif
