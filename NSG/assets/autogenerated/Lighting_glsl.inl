#pragma once
#include <string>
namespace NSG
{
static const std::string LIGHTING_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"struct Material\n"\
"{\n"\
"	vec4 color;\n"\
"	vec4 ambient;\n"\
"	vec4 diffuse;\n"\
"	vec4 specular;\n"\
"	float shininess;\n"\
"};\n"\
"struct BaseLight                                                                    \n"\
"{                \n"\
"	vec4 diffuse;\n"\
"	vec4 specular;\n"\
"};\n"\
"struct DirectionalLight\n"\
"{\n"\
"    BaseLight base;\n"\
"    vec3 direction;\n"\
"};\n"\
"struct Attenuation\n"\
"{\n"\
"    float constant;\n"\
"    float linear;\n"\
"    float quadratic;\n"\
"};                                                                \n"\
"                                                                                    \n"\
"struct PointLight                                                                           \n"\
"{                                                                                           \n"\
"    BaseLight base;                                                                         \n"\
"    vec3 position;                                                                          \n"\
"    Attenuation atten;                                                                      \n"\
"};\n"\
"uniform vec4 u_sceneAmbientColor;\n"\
"uniform vec3 u_eyeWorldPos;\n"\
"uniform Material u_material;\n"\
"uniform int u_numPointLights;\n"\
"uniform DirectionalLight u_directionalLight;                                                 \n"\
"uniform PointLight u_pointLights[MAX_POINT_LIGHTS];\n"\
"vec4 CalcLight(BaseLight light, vec3 vertexToEye, vec3 lightDirection, vec3 normal)                   \n"\
"{                                                                                           \n"\
"    vec4 color = u_sceneAmbientColor * u_material.ambient;\n"\
"	\n"\
"	float diffuseFactor = dot(normal, -lightDirection);	\n"\
"    if (diffuseFactor > 0.0) \n"\
"    {                                                                \n"\
"        color = max(color, diffuseFactor * light.diffuse * u_material.diffuse);                                \n"\
"        vec3 lightReflect = normalize(reflect(lightDirection, normal));\n"\
"        float specularFactor = dot(vertexToEye, lightReflect);\n"\
"        if (specularFactor > 0.0)\n"\
"       	{\n"\
"        	specularFactor = pow(specularFactor, u_material.shininess);\n"\
"            color += specularFactor * light.specular * u_material.specular;\n"\
"        }\n"\
"    }                                                                                       \n"\
"                                                                                            \n"\
"    return color;  \n"\
"}\n"\
"vec4 CalcDirectionalLight(vec3 vertexToEye, vec3 normal)\n"\
"{                                                                             \n"\
"    return CalcLight(u_directionalLight.base, vertexToEye, u_directionalLight.direction, normal);\n"\
"}\n"\
"vec4 CalcPointLight(int index, vec3 vertexToEye, vec3 normal)\n"\
"{                                                                                          \n"\
"    vec3 lightDirection = v_lightDirection[index];                         \n"\
"    float distance = length(lightDirection);                                                \n"\
"    lightDirection = normalize(lightDirection);                                             \n"\
"                                                                                            \n"\
"    vec4 color = CalcLight(u_pointLights[index].base, vertexToEye, lightDirection, normal);       \n"\
"    float attenuation =  u_pointLights[index].atten.constant +                               \n"\
"                         u_pointLights[index].atten.linear * distance +                      \n"\
"                         u_pointLights[index].atten.quadratic * distance * distance;               \n"\
"                                                                                            \n"\
"    return color / attenuation;                                                             \n"\
"}\n"\
;
}
