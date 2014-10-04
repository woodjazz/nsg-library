//Remember to rebuild with CMake if this file changes
struct Material
{
	vec4 color;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

uniform Material u_material;

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
    PointLight point;                                                                         
    vec3 direction;                                                                          
    float cutOff; // cosine(cutOff angle in c++)
};


uniform vec4 u_sceneAmbientColor;
uniform vec3 u_eyeWorldPos;

uniform DirectionalLight u_directionalLight[NUM_DIRECTIONAL_LIGHTS_ARRAY];
uniform PointLight u_pointLights[NUM_POINT_LIGHTS_ARRAY];
uniform SpotLight u_spotLights[NUM_SPOT_LIGHTS_ARRAY];

vec4 CalcLight(BaseLight base, vec3 vertexToEye, vec3 lightDirection, vec3 normal)                   
{                                                                                           
    vec4 color = u_material.ambient * base.ambient;
	
	float diffuseFactor = dot(normal, -lightDirection);	

    if (diffuseFactor > 0.0) 
    {                                                                
        color = max(color, diffuseFactor * base.diffuse * u_material.diffuse);                                
        vec3 lightReflect = normalize(reflect(lightDirection, normal));
        float specularFactor = dot(vertexToEye, lightReflect);
        if (specularFactor > 0.0)
       	{
        	specularFactor = pow(specularFactor, u_material.shininess);
            color += specularFactor * base.specular * u_material.specular;
        }
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
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);
    
    light2Pixel = normalize(light2Pixel);
    float spotFactor = dot(light2Pixel, lightDirection);

    if(spotFactor > cutOff)
    {
        vec4 color = CalcPointLight(base, lightDirection, attenuation, vertexToEye, normal);
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

    for (int i = 0 ; i < NUM_DIRECTIONAL_LIGHTS ; i++) 
    {
        if(u_directionalLight[i].enabled != 0)
            totalLight += CalcDirectionalLight(u_directionalLight[i].base, u_directionalLight[i].direction, vertexToEye, normal);
    }

    for (int i = 0 ; i < NUM_POINT_LIGHTS ; i++) 
    {
        if(u_pointLights[i].enabled != 0)
            totalLight += CalcPointLight(u_pointLights[i].base, v_lightDirection[i], u_pointLights[i].atten, vertexToEye, normal);                                            
    }

    for (int i = 0 ; i < NUM_SPOT_LIGHTS ; i++) 
    {
        if(u_spotLights[i].point.enabled != 0)
            totalLight += CalcSpotLight(u_spotLights[i].point.base, v_light2Pixel[i], u_spotLights[i].direction, u_spotLights[i].point.atten, vertexToEye, normal, u_spotLights[i].cutOff); 
    }

    return totalLight;
}

vec4 CalcVSTotalLight(vec3 worldPos, vec3 vertexToEye, vec3 normal)
{
    vec4 totalLight = u_sceneAmbientColor;

    for (int i = 0 ; i < NUM_DIRECTIONAL_LIGHTS ; i++)
    {
        if(u_directionalLight[i].enabled != 0)
            totalLight += CalcDirectionalLight(u_directionalLight[i].base, u_directionalLight[i].direction, vertexToEye, normal);
    }

    for (int i = 0 ; i < NUM_POINT_LIGHTS ; i++) 
    {
        if(u_pointLights[i].enabled != 0)
            totalLight += CalcPointLight(u_pointLights[i].base, worldPos - u_pointLights[i].position, u_pointLights[i].atten, vertexToEye, normal);
    }

    for (int i = 0 ; i < NUM_SPOT_LIGHTS ; i++) 
    {
        if(u_spotLights[i].point.enabled != 0)
            totalLight += CalcSpotLight(u_spotLights[i].point.base, worldPos - u_spotLights[i].point.position, u_spotLights[i].direction, u_spotLights[i].point.atten, vertexToEye, normal, u_spotLights[i].cutOff); 
    }

    return totalLight;
}

