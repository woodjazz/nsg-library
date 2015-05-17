#pragma once
namespace NSG
{
static const char* LIGHTING_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(COMPILEFS)\n"\
"vec4 GetAmbientIntensity()\n"\
"{\n"\
"    #if defined(AOMAP0)\n"\
"        #if defined(AOMAP_CHANNELS1)\n"\
"            vec4 intensity = vec4(texture2D(u_texture4, v_texcoord0).a);\n"\
"        #else\n"\
"            vec4 intensity = texture2D(u_texture4, v_texcoord0);\n"\
"        #endif\n"\
"    #elif defined(AOMAP1)\n"\
"        #if defined(AOMAP_CHANNELS1)\n"\
"            vec4 intensity = vec4(texture2D(u_texture4, v_texcoord1).a);\n"\
"        #else\n"\
"            vec4 intensity = texture2D(u_texture4, v_texcoord1);\n"\
"        #endif\n"\
"    #else\n"\
"        vec4 intensity = u_sceneAmbientColor * u_material.ambient;\n"\
"    #endif\n"\
"    #if defined(LIGHTMAP0)\n"\
"        #if defined(LIGHTMAP_CHANNELS1)\n"\
"            intensity *= vec4(texture2D(u_texture2, v_texcoord0).a);\n"\
"        #else\n"\
"            intensity *= texture2D(u_texture2, v_texcoord0);\n"\
"        #endif\n"\
"    #elif defined(LIGHTMAP1)\n"\
"        #if defined(LIGHTMAP_CHANNELS1)\n"\
"            intensity *= vec4(texture2D(u_texture2, v_texcoord1).a);\n"\
"        #else\n"\
"            intensity *= texture2D(u_texture2, v_texcoord1);\n"\
"        #endif\n"\
"    #endif\n"\
"    return intensity;\n"\
"}\n"\
"vec4 GetAmbientLight()\n"\
"{\n"\
"    #ifdef DIFFUSEMAP\n"\
"        vec4 diffColor = u_material.diffuse * texture2D(u_texture0, v_texcoord0);\n"\
"    #else\n"\
"        vec4 diffColor = u_material.diffuse;\n"\
"    #endif\n"\
"    return GetAmbientIntensity() * diffColor;\n"\
"}\n"\
"#endif\n"\
"#if defined(HAS_DIRECTIONAL_LIGHT) || defined(HAS_POINT_LIGHT) || defined(HAS_SPOT_LIGHT)\n"\
"vec4 CalcLight(BaseLight base, vec3 vertexToEye, vec3 lightDirection, vec3 normal)\n"\
"{\n"\
"    float diffuseFactor = clamp(dot(normal, -lightDirection), 0.0, 1.0);\n"\
"    vec4 color = diffuseFactor * base.diffuse * u_material.diffuse;\n"\
"    vec3 lightReflect = normalize(reflect(lightDirection, normal));\n"\
"    float specularFactor = clamp(dot(vertexToEye, lightReflect), 0.0, 1.0);\n"\
"    specularFactor = pow(specularFactor, u_material.shininess);\n"\
"    #if defined SPECULARMAP && defined(COMPILEFS)\n"\
"        color += specularFactor * base.specular * u_material.specular * texture2D(u_texture3, v_texcoord0);\n"\
"    #else\n"\
"        color += specularFactor * base.specular * u_material.specular;\n"\
"    #endif\n"\
"    return color;\n"\
"}\n"\
"#ifdef HAS_DIRECTIONAL_LIGHT\n"\
"vec4 CalcDirectionalLight(BaseLight base, vec3 lightDirection, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    return CalcLight(base, vertexToEye, lightDirection, normal);\n"\
"}\n"\
"#endif\n"\
"#if defined(HAS_POINT_LIGHT) || defined(HAS_SPOT_LIGHT)\n"\
"vec4 CalcPointLight(BaseLight base, vec3 lightDirection, float invRange, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    float lightDist = length(lightDirection * invRange);\n"\
"    lightDirection = normalize(lightDirection);\n"\
"    vec4 color = CalcLight(base, vertexToEye, lightDirection, normal);\n"\
"    float atten = clamp(1.0 - lightDist * lightDist, 0.0, 1.0);\n"\
"    return color * atten;\n"\
"}\n"\
"#endif\n"\
"#ifdef HAS_SPOT_LIGHT\n"\
"vec4 CalcSpotLight(BaseLight base, vec3 light2Pixel, vec3 lightDirection, float invRange, vec3 vertexToEye, vec3 normal, float cutOff)\n"\
"{\n"\
"    lightDirection = normalize(lightDirection);\n"\
"    vec4 color = CalcPointLight(base, light2Pixel, invRange, vertexToEye, normal);\n"\
"    light2Pixel = normalize(light2Pixel);\n"\
"    float spotFactor = dot(light2Pixel, lightDirection);\n"\
"    float spotAtten = clamp((spotFactor - cutOff) / (1.0 - cutOff), 0.0, 1.0);\n"\
"    return color * spotAtten;\n"\
"}\n"\
"#endif\n"\
"vec4 CalcFSTotalLight(vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    #if defined(HAS_DIRECTIONAL_LIGHT)\n"\
"        return u_sceneAmbientColor + CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);\n"\
"    #elif defined(HAS_POINT_LIGHT)\n"\
"        return u_sceneAmbientColor + CalcPointLight(u_pointLight.base, v_lightDirection, u_pointLight.invRange, vertexToEye, normal);\n"\
"    #elif defined(HAS_SPOT_LIGHT)\n"\
"        return u_sceneAmbientColor + CalcSpotLight(u_spotLight.base, v_lightDirection, u_spotLight.direction, u_spotLight.invRange, vertexToEye, normal, u_spotLight.cutOff);\n"\
"    #endif\n"\
"}\n"\
"vec4 CalcVSTotalLight(vec3 worldPos, vec3 vertexToEye, vec3 normal)\n"\
"{\n"\
"    #if defined(HAS_DIRECTIONAL_LIGHT)\n"\
"        return u_sceneAmbientColor + CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);\n"\
"    #elif defined(HAS_POINT_LIGHT)\n"\
"        return u_sceneAmbientColor + CalcPointLight(u_pointLight.base, worldPos - u_pointLight.position, u_pointLight.invRange, vertexToEye, normal);\n"\
"    #elif defined(HAS_SPOT_LIGHT)\n"\
"        return u_sceneAmbientColor + CalcSpotLight(u_spotLight.base, worldPos - u_spotLight.position, u_spotLight.direction, u_spotLight.invRange, vertexToEye, normal, u_spotLight.cutOff);\n"\
"    #endif\n"\
"}\n"\
"#endif\n"\
;
}
