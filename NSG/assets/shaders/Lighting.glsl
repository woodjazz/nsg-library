//Remember to rebuild with CMake if this file changes
struct Material
{
	vec4 color;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shininess;
};

struct BaseLight                                                                    
{                
	vec4 diffuse;
	vec4 specular;
};

struct DirectionalLight
{
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
    BaseLight base;                                                                         
    vec3 position;                                                                          
    Attenuation atten;                                                                      
};

uniform vec4 u_sceneAmbientColor;
uniform vec3 u_eyeWorldPos;
uniform Material u_material;
uniform int u_numPointLights;
uniform DirectionalLight u_directionalLight;                                                 
uniform PointLight u_pointLights[MAX_POINT_LIGHTS];

vec4 CalcLight(BaseLight light, vec3 vertexToEye, vec3 lightDirection, vec3 normal)                   
{                                                                                           
    vec4 color = u_sceneAmbientColor * u_material.ambient;
	
	float diffuseFactor = dot(normal, -lightDirection);	

    if (diffuseFactor > 0.0) 
    {                                                                
        color = max(color, diffuseFactor * light.diffuse * u_material.diffuse);                                
        vec3 lightReflect = normalize(reflect(lightDirection, normal));
        float specularFactor = dot(vertexToEye, lightReflect);
        if (specularFactor > 0.0)
       	{
        	specularFactor = pow(specularFactor, u_material.shininess);
            color += specularFactor * light.specular * u_material.specular;
        }
    }                                                                                       
                                                                                            
    return color;  
}

vec4 CalcDirectionalLight(vec3 vertexToEye, vec3 normal)
{                                                                             
    return CalcLight(u_directionalLight.base, vertexToEye, u_directionalLight.direction, normal);
}

vec4 CalcPointLight(int index, vec3 vertexToEye, vec3 normal)
{                                                                                          
    vec3 lightDirection = v_lightDirection[index];                         
    float distance = length(lightDirection);                                                
    lightDirection = normalize(lightDirection);                                             
                                                                                            
    vec4 color = CalcLight(u_pointLights[index].base, vertexToEye, lightDirection, normal);       
    float attenuation =  u_pointLights[index].atten.constant +                               
                         u_pointLights[index].atten.linear * distance +                      
                         u_pointLights[index].atten.quadratic * distance * distance;               
                                                                                            
    return color / attenuation;                                                             
}
