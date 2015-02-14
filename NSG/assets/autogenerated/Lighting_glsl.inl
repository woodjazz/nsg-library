#pragma once
namespace NSG
{
static const char* LIGHTING_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#ifdef HAS_LIGHTS\n"\
"vec4 CalcLight(BaseLight base, vec3 vertexToEye, vec3 lightDirection, vec3 normal)\n"\
"{\n"\
"    #ifdef AOMAP\n"\
"    vec4 color = texture2D(u_texture4, v_texcoord0);\n"\
"    #else\n"\
"    vec4 color = u_material.ambient * base.ambient;\n"\
"    #endif\n"\
"    float diffuseFactor = clamp(dot(normal, -lightDirection), 0.0, 1.0);\n"\
"    color = max(color, diffuseFactor * base.diffuse * u_material.diffuse);\n"\
"    vec3 lightReflect = normalize(reflect(lightDirection, normal));\n"\
"    float specularFactor = clamp(dot(vertexToEye, lightReflect), 0.0, 1.0);\n"\
"    specularFactor = pow(specularFactor, u_material.shininess);\n"\
"    #ifdef SPECULARMAP\n"\
"    color += specularFactor * base.specular * u_material.specular * texture2D(u_texture3, v_texcoord0);\n"\
"    #else\n"\
"    color += specularFactor * base.specular * u_material.specular;\n"\
"    #endif\n"\
"    return color;\n"\
"}\n"\
"#ifdef HAS_DIRECTIONAL_LIGHTS\n"\
"vec4 CalcDirectionalLight(BaseLight base, vec3 lightDirection, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    return CalcLight(base, vertexToEye, lightDirection, normal);\n"\
"}\n"\
"#endif\n"\
"#if defined(HAS_POINT_LIGHTS) || defined(HAS_SPOT_LIGHTS)\n"\
"vec4 CalcPointLight(BaseLight base, vec3 lightDirection, Attenuation attenuation, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    float distance = length(lightDirection);\n"\
"    lightDirection = normalize(lightDirection);\n"\
"    vec4 color = CalcLight(base, vertexToEye, lightDirection, normal);\n"\
"    float attenuationFactor =  attenuation.constant + attenuation.linear * distance + attenuation.quadratic * distance * distance;\n"\
"    return color / attenuationFactor;\n"\
"}\n"\
"#endif\n"\
"#ifdef HAS_SPOT_LIGHTS\n"\
"vec4 CalcSpotLight(BaseLight base, vec3 light2Pixel, vec3 lightDirection, Attenuation attenuation, vec3 vertexToEye, vec3 normal, float cutOff)\n"\
"{\n"\
"    lightDirection = normalize(lightDirection);\n"\
"    light2Pixel = normalize(light2Pixel);\n"\
"    float spotFactor = dot(light2Pixel, lightDirection);\n"\
"    if (spotFactor > cutOff)\n"\
"    {\n"\
"        vec4 color = CalcPointLight(base, light2Pixel, attenuation, vertexToEye, normal);\n"\
"        return color * (1.0 - (1.0 - spotFactor) * 1.0 / (1.0 - cutOff));\n"\
"    }\n"\
"    else\n"\
"    {\n"\
"        return vec4(0.0);\n"\
"    }\n"\
"}\n"\
"#endif\n"\
"vec4 CalcFSTotalLight(vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    vec4 totalLight = u_sceneAmbientColor;\n"\
"    #ifdef HAS_DIRECTIONAL_LIGHTS\n"\
"    for (int i = 0 ; i < NUM_DIRECTIONAL_LIGHTS ; i++)\n"\
"    {\n"\
"        if (u_directionalLight[i].enabled != 0)\n"\
"            totalLight += CalcDirectionalLight(u_directionalLight[i].base, u_directionalLight[i].direction, vertexToEye, normal);\n"\
"    }\n"\
"    #endif\n"\
"    #ifdef HAS_POINT_LIGHTS\n"\
"    for (int i = 0 ; i < NUM_POINT_LIGHTS ; i++)\n"\
"    {\n"\
"        if (u_pointLights[i].enabled != 0)\n"\
"            totalLight += CalcPointLight(u_pointLights[i].base, v_lightDirection[i], u_pointLights[i].atten, vertexToEye, normal);\n"\
"    }\n"\
"    #endif\n"\
"    #ifdef HAS_SPOT_LIGHTS\n"\
"    for (int i = 0 ; i < NUM_SPOT_LIGHTS ; i++)\n"\
"    {\n"\
"        if (u_spotLights[i].enabled != 0)\n"\
"            totalLight += CalcSpotLight(u_spotLights[i].base, v_light2Pixel[i], u_spotLights[i].direction, u_spotLights[i].atten, vertexToEye, normal, u_spotLights[i].cutOff);\n"\
"    }\n"\
"    #endif\n"\
"    return totalLight;\n"\
"}\n"\
"vec4 CalcVSTotalLight(vec3 worldPos, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    vec4 totalLight = u_sceneAmbientColor;\n"\
"    #ifdef HAS_DIRECTIONAL_LIGHTS\n"\
"    for (int i = 0 ; i < NUM_DIRECTIONAL_LIGHTS ; i++)\n"\
"    {\n"\
"        if (u_directionalLight[i].enabled != 0)\n"\
"            totalLight += CalcDirectionalLight(u_directionalLight[i].base, u_directionalLight[i].direction, vertexToEye, normal);\n"\
"    }\n"\
"    #endif\n"\
"    #ifdef HAS_POINT_LIGHTS\n"\
"    for (int i = 0 ; i < NUM_POINT_LIGHTS ; i++)\n"\
"    {\n"\
"        if (u_pointLights[i].enabled != 0)\n"\
"            totalLight += CalcPointLight(u_pointLights[i].base, worldPos - u_pointLights[i].position, u_pointLights[i].atten, vertexToEye, normal);\n"\
"    }\n"\
"    #endif\n"\
"    #ifdef HAS_SPOT_LIGHTS\n"\
"    for (int i = 0 ; i < NUM_SPOT_LIGHTS ; i++)\n"\
"    {\n"\
"        if (u_spotLights[i].enabled != 0)\n"\
"            totalLight += CalcSpotLight(u_spotLights[i].base, worldPos - u_spotLights[i].position, u_spotLights[i].direction, u_spotLights[i].atten, vertexToEye, normal, u_spotLights[i].cutOff);\n"\
"    }\n"\
"    #endif\n"\
"    return totalLight;\n"\
"}\n"\
"#endif\n"\
;
}
