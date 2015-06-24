#pragma once
namespace NSG
{
static const char* LIGHTING_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"vec4 CalcLight(vec3 lightDirection, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    float dFactor = clamp(dot(normal, -lightDirection), 0.0, 1.0);\n"\
"    #ifdef DIFFUSEMAP\n"\
"        vec4 diffuse = dFactor * u_lightDiffuseColor;\n"\
"    #else\n"\
"        vec4 diffuse = dFactor * u_lightDiffuseColor * u_material.diffuse;\n"\
"    #endif\n"\
"    #ifdef SPECULAR\n"\
"        vec3 lightReflect = normalize(reflect(lightDirection, normal));\n"\
"        float sFactor = clamp(dot(vertexToEye, lightReflect), 0.0, 1.0);\n"\
"        sFactor = pow(sFactor, u_material.shininess);\n"\
"        #if defined SPECULARMAP && defined(COMPILEFS)\n"\
"            vec4 specular = sFactor * u_lightSpecularColor * u_material.specular * texture2D(u_texture2, v_texcoord0);\n"\
"        #else\n"\
"            vec4 specular = sFactor * u_lightSpecularColor * u_material.specular;\n"\
"        #endif\n"\
"        return diffuse + specular;\n"\
"    #else\n"\
"        return diffuse;\n"\
"    #endif\n"\
"}\n"\
"vec4 CalcPointLight(vec3 lightDirection, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    float lightDist = length(lightDirection * u_lightInvRange[0]);\n"\
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
"            return CalcShadowFactor(world2light) * CalcLight(u_lightDirection, vertexToEye, normal);\n"\
"        #elif defined(HAS_POINT_LIGHT)\n"\
"            return CalcShadowFactor(world2light) * CalcPointLight(world2light, vertexToEye, normal);\n"\
"        #elif defined(HAS_SPOT_LIGHT)\n"\
"            return CalcShadowFactor(world2light) * CalcSpotLight(world2light, vertexToEye, normal);\n"\
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
;
}
