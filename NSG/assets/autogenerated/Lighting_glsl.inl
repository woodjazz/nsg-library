#pragma once
namespace NSG
{
static const char* LIGHTING_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(COMPILEFS) && !defined(SHADOW_DIR_PASS) && !defined(SHADOW_POINT_PASS) && !defined(SHADOW_SPOT_PASS)\n"\
"float GetSpecular(vec3 normal, vec3 eyeVec, vec3 lightDir, float specularPower)\n"\
"{\n"\
"    vec3 halfVec = normalize(normalize(eyeVec) + lightDir);\n"\
"    return pow(max(dot(normal, halfVec), 0.0), specularPower);\n"\
"}\n"\
"vec4 CalcLight(vec3 lightDirection, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    float dFactor = clamp(dot(normal, -lightDirection), 0.0, 1.0);\n"\
"    vec4 diffuse = dFactor * u_lightDiffuseColor * GetDiffuseColor();\n"\
"    #if defined(SPECULAR)\n"\
"        float sFactor = GetSpecular(normal, vertexToEye, -lightDirection, u_material.shininess);\n"\
"        vec4 specular = sFactor * u_lightSpecularColor * GetSpecularColor();\n"\
"        return diffuse + specular;\n"\
"    #else\n"\
"        return diffuse;\n"\
"    #endif\n"\
"}\n"\
"vec4 CalcPointLight(vec3 lightDirection, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    float lightDist = length(lightDirection * u_lightInvRange);\n"\
"    lightDirection = normalize(lightDirection);\n"\
"    vec4 color = CalcLight(lightDirection, vertexToEye, normal);\n"\
"    float atten = clamp(1.0 - lightDist * lightDist, 0.0, 1.0);\n"\
"    return color * atten;\n"\
"}\n"\
"vec4 CalcSpotLight(vec3 light2Pixel, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    vec4 color = CalcPointLight(light2Pixel, vertexToEye, normal);\n"\
"    light2Pixel = normalize(light2Pixel);\n"\
"    float spotFactor = dot(light2Pixel, u_lightDirection);\n"\
"    float spotAtten = clamp((spotFactor - u_lightCutOff) / (1.0 - u_lightCutOff), 0.0, 1.0);\n"\
"    return color * spotAtten;\n"\
"}\n"\
"vec4 CalcTotalLight(vec3 world2light, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    #if defined(COMPILEFS) && (defined(SHADOWMAP) || defined(CUBESHADOWMAP))\n"\
"        #if defined(HAS_DIRECTIONAL_LIGHT)\n"\
"            return CalcShadowFactor() * CalcLight(u_lightDirection, vertexToEye, normal);\n"\
"        #elif defined(HAS_POINT_LIGHT)\n"\
"            return CalcShadowFactor(world2light) * CalcPointLight(world2light, vertexToEye, normal);\n"\
"        #elif defined(HAS_SPOT_LIGHT)\n"\
"            return CalcShadowFactor() * CalcSpotLight(world2light, vertexToEye, normal);\n"\
"        #else \n"\
"            return vec4(1.0);        \n"\
"        #endif\n"\
"    #else\n"\
"        #if defined(HAS_DIRECTIONAL_LIGHT)\n"\
"            return CalcLight(u_lightDirection, vertexToEye, normal);\n"\
"        #elif defined(HAS_POINT_LIGHT)\n"\
"            return CalcPointLight(world2light, vertexToEye, normal);\n"\
"        #elif defined(HAS_SPOT_LIGHT)\n"\
"            return CalcSpotLight(world2light, vertexToEye, normal);\n"\
"        #else \n"\
"            return vec4(1.0);        \n"\
"        #endif\n"\
"    #endif\n"\
"}\n"\
"#endif\n"\
;
}
