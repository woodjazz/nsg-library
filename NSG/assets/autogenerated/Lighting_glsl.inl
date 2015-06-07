#pragma once
namespace NSG
{
static const char* LIGHTING_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(COMPILEFS)\n"\
"    #if defined(AMBIENT_PASS)\n"\
"        vec4 GetAmbientIntensity()\n"\
"        {\n"\
"            #if !defined(AOMAP0) && !defined(AOMAP1) && !defined(LIGHTMAP0) && !defined(LIGHTMAP1)\n"\
"                #if defined(UNLIT)\n"\
"                    vec4 intensity = u_material.color;\n"\
"                #else\n"\
"                    vec4 intensity = u_sceneAmbientColor * u_material.ambient;\n"\
"                #endif\n"\
"            #else\n"\
"                #if defined(AOMAP0)\n"\
"                    #if defined(AOMAP_CHANNELS1)\n"\
"                        vec4 intensity = vec4(texture2D(u_texture4, v_texcoord0).a);\n"\
"                    #else\n"\
"                        vec4 intensity = texture2D(u_texture4, v_texcoord0);\n"\
"                    #endif\n"\
"                #elif defined(AOMAP1)\n"\
"                    #if defined(AOMAP_CHANNELS1)\n"\
"                        vec4 intensity = vec4(texture2D(u_texture4, v_texcoord1).a);\n"\
"                    #else\n"\
"                        vec4 intensity = texture2D(u_texture4, v_texcoord1);\n"\
"                    #endif\n"\
"                #else\n"\
"                    vec4 intensity = vec4(1.0);\n"\
"                #endif\n"\
"                #if defined(LIGHTMAP0)\n"\
"                    #if defined(LIGHTMAP_CHANNELS1)\n"\
"                        intensity *= vec4(texture2D(u_texture2, v_texcoord0).a);\n"\
"                    #else\n"\
"                        intensity *= texture2D(u_texture2, v_texcoord0);\n"\
"                    #endif\n"\
"                #elif defined(LIGHTMAP1)\n"\
"                    #if defined(LIGHTMAP_CHANNELS1)\n"\
"                        intensity *= vec4(texture2D(u_texture2, v_texcoord1).a);\n"\
"                    #else\n"\
"                        intensity *= texture2D(u_texture2, v_texcoord1);\n"\
"                    #endif\n"\
"                #endif\n"\
"            #endif\n"\
"            return intensity;\n"\
"        }\n"\
"        vec4 GetAmbientLight()\n"\
"        {\n"\
"            #ifdef DIFFUSEMAP\n"\
"                return GetAmbientIntensity() * texture2D(u_texture0, v_texcoord0);\n"\
"            #else\n"\
"                return GetAmbientIntensity();\n"\
"            #endif\n"\
"        }\n"\
"    #endif\n"\
"#endif\n"\
"#if defined(HAS_DIRECTIONAL_LIGHT) || defined(HAS_POINT_LIGHT) || defined(HAS_SPOT_LIGHT)\n"\
"    vec4 CalcLight(BaseLight base, vec3 vertexToEye, vec3 lightDirection, vec3 normal)\n"\
"    {\n"\
"        float diffuseFactor = clamp(dot(normal, -lightDirection), 0.0, 1.0);\n"\
"        vec4 color = diffuseFactor * base.diffuse * u_material.diffuse;\n"\
"        vec3 lightReflect = normalize(reflect(lightDirection, normal));\n"\
"        float specularFactor = clamp(dot(vertexToEye, lightReflect), 0.0, 1.0);\n"\
"        specularFactor = pow(specularFactor, u_material.shininess);\n"\
"        #if defined SPECULARMAP && defined(COMPILEFS)\n"\
"            color += specularFactor * base.specular * u_material.specular * texture2D(u_texture3, v_texcoord0);\n"\
"        #else\n"\
"            color += specularFactor * base.specular * u_material.specular;\n"\
"        #endif\n"\
"        return color;\n"\
"    }\n"\
"    #ifdef HAS_DIRECTIONAL_LIGHT\n"\
"    vec4 CalcDirectionalLight(BaseLight base, vec3 lightDirection, vec3 vertexToEye, vec3 normal)\n"\
"    {\n"\
"        return CalcLight(base, vertexToEye, lightDirection, normal);\n"\
"    }\n"\
"    #endif\n"\
"    #if defined(HAS_POINT_LIGHT) || defined(HAS_SPOT_LIGHT)\n"\
"    vec4 CalcPointLight(BaseLight base, vec3 lightDirection, vec3 vertexToEye, vec3 normal)\n"\
"    {\n"\
"        float lightDist = length(lightDirection * u_lightInvRange);\n"\
"        lightDirection = normalize(lightDirection);\n"\
"        vec4 color = CalcLight(base, vertexToEye, lightDirection, normal);\n"\
"        float atten = clamp(1.0 - lightDist * lightDist, 0.0, 1.0);\n"\
"        return color * atten;\n"\
"    }\n"\
"    #endif\n"\
"    #ifdef HAS_SPOT_LIGHT\n"\
"    vec4 CalcSpotLight(BaseLight base, vec3 light2Pixel, vec3 lightDirection, vec3 vertexToEye, vec3 normal, float cutOff)\n"\
"    {\n"\
"        lightDirection = normalize(lightDirection);\n"\
"        vec4 color = CalcPointLight(base, light2Pixel, vertexToEye, normal);\n"\
"        light2Pixel = normalize(light2Pixel);\n"\
"        float spotFactor = dot(light2Pixel, lightDirection);\n"\
"        float spotAtten = clamp((spotFactor - cutOff) / (1.0 - cutOff), 0.0, 1.0);\n"\
"        return color * spotAtten;\n"\
"    }\n"\
"    #endif\n"\
"    #if defined(COMPILEFS)\n"\
"        #if defined(CUBESHADOWMAP)\n"\
"            vec3 FixCubeLookup(vec3 v) \n"\
"            {\n"\
"                // To eliminate the edge seams\n"\
"                // Since the extension ARB_seamless_cube_map is not always available.\n"\
"                // From http://the-witness.net/news/2012/02/seamless-cube-map-filtering \n"\
"                float cube_size = 1.0/u_shadowMapInvSize;\n"\
"                float M = max(max(abs(v.x), abs(v.y)), abs(v.z)); \n"\
"                float scale = (cube_size - 1.0) / cube_size; \n"\
"                if (abs(v.x) != M) v.x *= scale; \n"\
"                if (abs(v.y) != M) v.y *= scale; \n"\
"                if (abs(v.z) != M) v.z *= scale; \n"\
"                return v; \n"\
"            }\n"\
"            vec4 CalcShadowCubeFactor()\n"\
"            {\n"\
"                float lengthLightDirection = length(v_lightDirection);\n"\
"                float totalError = u_shadowBias * lengthLightDirection;\n"\
"                float sampledDistance = DecodeColor2Depth(textureCube(u_texture5, FixCubeLookup(v_lightDirection))) / u_lightInvRange;\n"\
"                return sampledDistance + totalError < lengthLightDirection ? u_shadowColor : vec4(1.0);\n"\
"            }\n"\
"        #elif defined(SHADOWMAP)\n"\
"            vec4 CalcShadowFactor()\n"\
"            {\n"\
"                float lengthLightDirection = length(v_lightDirection);\n"\
"                float totalError = u_shadowBias * lengthLightDirection;\n"\
"                float lightToPixelDistance = clamp(lengthLightDirection * u_lightInvRange, 0.0, 1.0);\n"\
"                vec4 coords = v_shadowClipPos / v_shadowClipPos.w; // Normalize from -w..w to -1..1\n"\
"                coords = 0.5 * coords + 0.5; // Normalize from -1..1 to 0..1\n"\
"                // Take four samples and average them\n"\
"                float sampledDistance = DecodeColor2Depth(texture2D(u_texture5, coords.xy));\n"\
"                sampledDistance += DecodeColor2Depth(texture2D(u_texture5, coords.xy + vec2(u_shadowMapInvSize, 0.0)));\n"\
"                sampledDistance += DecodeColor2Depth(texture2D(u_texture5, coords.xy + vec2(0.0, u_shadowMapInvSize)));\n"\
"                sampledDistance += DecodeColor2Depth(texture2D(u_texture5, coords.xy + vec2(u_shadowMapInvSize)));\n"\
"                sampledDistance *= 0.25;\n"\
"                return sampledDistance + totalError < lightToPixelDistance ? u_shadowColor : vec4(1.0);\n"\
"            }\n"\
"        #endif\n"\
"        vec4 CalcTotalLight(vec3 vertexToEye, vec3 normal)\n"\
"        {\n"\
"            #if defined(HAS_DIRECTIONAL_LIGHT)\n"\
"                #if defined(SHADOWMAP)\n"\
"                    return CalcShadowFactor() * CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);\n"\
"                #else\n"\
"                    return CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);\n"\
"                #endif                    \n"\
"            #elif defined(HAS_POINT_LIGHT)\n"\
"                #if defined(CUBESHADOWMAP)\n"\
"                    return CalcShadowCubeFactor() * CalcPointLight(u_pointLight.base, v_lightDirection, vertexToEye, normal);\n"\
"                #else\n"\
"                    return CalcPointLight(u_pointLight.base, v_lightDirection, vertexToEye, normal);\n"\
"                #endif\n"\
"            #elif defined(HAS_SPOT_LIGHT)\n"\
"                #if defined(SHADOWMAP)\n"\
"                    return CalcShadowFactor() * CalcSpotLight(u_spotLight.base, v_lightDirection, u_spotLight.direction, vertexToEye, normal, u_spotLight.cutOff);\n"\
"                #else\n"\
"                    return CalcSpotLight(u_spotLight.base, v_lightDirection, u_spotLight.direction, vertexToEye, normal, u_spotLight.cutOff);\n"\
"                #endif\n"\
"            #endif\n"\
"        }\n"\
"    #elif defined(COMPILEVS)\n"\
"        vec4 CalcTotalLight(vec3 worldPos, vec3 vertexToEye, vec3 normal)\n"\
"        {\n"\
"            #if defined(HAS_DIRECTIONAL_LIGHT)\n"\
"                return CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);\n"\
"            #elif defined(HAS_POINT_LIGHT)\n"\
"                return CalcPointLight(u_pointLight.base, worldPos - u_pointLight.position, vertexToEye, normal);\n"\
"            #elif defined(HAS_SPOT_LIGHT)\n"\
"                return CalcSpotLight(u_spotLight.base, worldPos - u_spotLight.position, u_spotLight.direction, vertexToEye, normal, u_spotLight.cutOff);\n"\
"            #endif\n"\
"        }\n"\
"    #endif    \n"\
"#endif\n"\
;
}
