#pragma once
namespace NSG
{
static const char* LIGHTING_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"struct Material\n"\
"{\n"\
"    vec4 color;\n"\
"    vec4 ambient;\n"\
"    vec4 diffuse;\n"\
"    vec4 specular;\n"\
"    float shininess;\n"\
"    float parallaxScale;\n"\
"};\n"\
"uniform Material u_material;\n"\
"#ifdef HAS_LIGHTS\n"\
"struct BaseLight\n"\
"{\n"\
"    vec4 ambient;      \n"\
"	vec4 diffuse;\n"\
"	vec4 specular;\n"\
"};\n"\
"struct DirectionalLight\n"\
"{\n"\
"    int enabled;\n"\
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
"struct PointLight                                                                      \n"\
"{\n"\
"    int enabled;\n"\
"    BaseLight base;\n"\
"    vec3 position;\n"\
"    Attenuation atten;\n"\
"};\n"\
"struct SpotLight                                                                      \n"\
"{\n"\
"    int enabled;                                                                          \n"\
"    PointLight point;                                                                         \n"\
"    vec3 direction;                                                                          \n"\
"    float cutOff; // cosine(cutOff angle in c++)\n"\
"};\n"\
"uniform vec4 u_sceneAmbientColor;\n"\
"uniform vec3 u_eyeWorldPos;\n"\
"#ifdef HAS_DIRECTIONAL_LIGHTS\n"\
"    uniform DirectionalLight u_directionalLight[NUM_DIRECTIONAL_LIGHTS];\n"\
"#endif\n"\
"#ifdef HAS_POINT_LIGHTS    \n"\
"    uniform PointLight u_pointLights[NUM_POINT_LIGHTS];\n"\
"#endif\n"\
"#ifdef HAS_SPOT_LIGHTS    \n"\
"    uniform SpotLight u_spotLights[NUM_SPOT_LIGHTS];\n"\
"#endif    \n"\
"vec4 CalcLight(BaseLight base, vec3 vertexToEye, vec3 lightDirection, vec3 normal)                   \n"\
"{                               \n"\
"    #ifdef AOMAP\n"\
"        vec4 color = u_material.ambient * base.ambient * texture2D(u_texture4, v_texcoord0);\n"\
"    #else                                                            \n"\
"        vec4 color = u_material.ambient * base.ambient;\n"\
"    #endif\n"\
"	\n"\
"	float diffuseFactor = dot(normal, -lightDirection);	\n"\
"    if (diffuseFactor > 0.0) \n"\
"    {                                                                \n"\
"        color = max(color, diffuseFactor * base.diffuse * u_material.diffuse);                                \n"\
"        vec3 lightReflect = normalize(reflect(lightDirection, normal));\n"\
"        float specularFactor = dot(vertexToEye, lightReflect);\n"\
"        if (specularFactor > 0.0)\n"\
"       	{\n"\
"        	specularFactor = pow(specularFactor, u_material.shininess);\n"\
"            #ifdef SPECULARMAP\n"\
"                color += specularFactor * base.specular * u_material.specular * texture2D(u_texture3, v_texcoord0);\n"\
"            #else\n"\
"                color += specularFactor * base.specular * u_material.specular;\n"\
"            #endif\n"\
"        }\n"\
"    }                                                                                       \n"\
"                                                                            \n"\
"    return color;  \n"\
"}\n"\
"vec4 CalcDirectionalLight(BaseLight base, vec3 lightDirection, vec3 vertexToEye, vec3 normal)\n"\
"{                                                                             \n"\
"    return CalcLight(base, vertexToEye, lightDirection, normal);\n"\
"}\n"\
"vec4 CalcPointLight(BaseLight base, vec3 lightDirection, Attenuation attenuation, vec3 vertexToEye, vec3 normal)\n"\
"{                                                                                          \n"\
"    float distance = length(lightDirection);\n"\
"    lightDirection = normalize(lightDirection);\n"\
"    vec4 color = CalcLight(base, vertexToEye, lightDirection, normal);       \n"\
"    float attenuationFactor =  attenuation.constant + attenuation.linear * distance + attenuation.quadratic * distance * distance;\n"\
"    return color / attenuationFactor;\n"\
"}\n"\
"vec4 CalcSpotLight(BaseLight base, vec3 light2Pixel, vec3 lightDirection, Attenuation attenuation, vec3 vertexToEye, vec3 normal, float cutOff)\n"\
"{\n"\
"    float distance = length(lightDirection);\n"\
"    lightDirection = normalize(lightDirection);\n"\
"    \n"\
"    light2Pixel = normalize(light2Pixel);\n"\
"    float spotFactor = dot(light2Pixel, lightDirection);\n"\
"    if(spotFactor > cutOff)\n"\
"    {\n"\
"        vec4 color = CalcPointLight(base, lightDirection, attenuation, vertexToEye, normal);\n"\
"        return color * (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - cutOff));\n"\
"    }\n"\
"    else\n"\
"    {\n"\
"        return vec4(0.0);\n"\
"    }\n"\
"}\n"\
"vec4 CalcFSTotalLight(vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    vec4 totalLight = u_sceneAmbientColor;\n"\
"    #ifdef HAS_DIRECTIONAL_LIGHTS\n"\
"        for (int i = 0 ; i < NUM_DIRECTIONAL_LIGHTS ; i++) \n"\
"        {\n"\
"            if(u_directionalLight[i].enabled != 0)\n"\
"                totalLight += CalcDirectionalLight(u_directionalLight[i].base, u_directionalLight[i].direction, vertexToEye, normal);\n"\
"        }\n"\
"    #endif\n"\
"    #ifdef HAS_POINT_LIGHTS\n"\
"        for (int i = 0 ; i < NUM_POINT_LIGHTS ; i++) \n"\
"        {\n"\
"            if(u_pointLights[i].enabled != 0)\n"\
"                totalLight += CalcPointLight(u_pointLights[i].base, v_lightDirection[i], u_pointLights[i].atten, vertexToEye, normal);                                            \n"\
"        }\n"\
"    #endif\n"\
"    #ifdef HAS_SPOT_LIGHTS\n"\
"        for (int i = 0 ; i < NUM_SPOT_LIGHTS ; i++) \n"\
"        {\n"\
"            if(u_spotLights[i].point.enabled != 0)\n"\
"                totalLight += CalcSpotLight(u_spotLights[i].point.base, v_light2Pixel[i], u_spotLights[i].direction, u_spotLights[i].point.atten, vertexToEye, normal, u_spotLights[i].cutOff); \n"\
"        }\n"\
"    #endif\n"\
"    return totalLight;\n"\
"}\n"\
"vec4 CalcVSTotalLight(vec3 worldPos, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    vec4 totalLight = u_sceneAmbientColor;\n"\
"    #ifdef HAS_DIRECTIONAL_LIGHTS\n"\
"        for (int i = 0 ; i < NUM_DIRECTIONAL_LIGHTS ; i++)\n"\
"        {\n"\
"            if(u_directionalLight[i].enabled != 0)\n"\
"                totalLight += CalcDirectionalLight(u_directionalLight[i].base, u_directionalLight[i].direction, vertexToEye, normal);\n"\
"        }\n"\
"    #endif\n"\
"    #ifdef HAS_POINT_LIGHTS\n"\
"        for (int i = 0 ; i < NUM_POINT_LIGHTS ; i++) \n"\
"        {\n"\
"            if(u_pointLights[i].enabled != 0)\n"\
"                totalLight += CalcPointLight(u_pointLights[i].base, worldPos - u_pointLights[i].position, u_pointLights[i].atten, vertexToEye, normal);\n"\
"        }\n"\
"    #endif\n"\
"    #ifdef HAS_SPOT_LIGHTS\n"\
"        for (int i = 0 ; i < NUM_SPOT_LIGHTS ; i++) \n"\
"        {\n"\
"            if(u_spotLights[i].point.enabled != 0)\n"\
"                totalLight += CalcSpotLight(u_spotLights[i].point.base, worldPos - u_spotLights[i].point.position, u_spotLights[i].direction, u_spotLights[i].point.atten, vertexToEye, normal, u_spotLights[i].cutOff); \n"\
"        }\n"\
"    #endif\n"\
"    return totalLight;\n"\
"}\n"\
"#endif\n"\
;
}
