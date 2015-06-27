//Remember to rebuild with CMake if this file changes

vec4 CalcLight(vec3 lightDirection, vec3 vertexToEye, vec3 normal)
{
    float dFactor = clamp(dot(normal, -lightDirection), 0.0, 1.0);
    vec4 diffuse = dFactor * u_lightDiffuseColor;
    #ifdef SPECULAR
        vec3 lightReflect = normalize(reflect(lightDirection, normal));
        float sFactor = clamp(dot(vertexToEye, lightReflect), 0.0, 1.0);
        sFactor = pow(sFactor, u_material.shininess);
        #if defined SPECULARMAP && defined(COMPILEFS)
            vec4 specularMap = texture2D(u_texture2, v_texcoord0);
            vec4 specular = sFactor * u_lightSpecularColor * vec4(specularMap.rgb, specularMap.a + u_material.specular.a);
        #else
            vec4 specular = sFactor * u_lightSpecularColor * u_material.specular;
        #endif
        return diffuse + specular;
    #else
        return diffuse;
    #endif
}

vec4 CalcPointLight(vec3 lightDirection, vec3 vertexToEye, vec3 normal)
{
    float lightDist = length(lightDirection * u_lightInvRange[0]);
    lightDirection = normalize(lightDirection);
    vec4 color = CalcLight(lightDirection, vertexToEye, normal);
    float atten = clamp(1.0 - lightDist * lightDist, 0.0, 1.0);
    return color * atten;
}

vec4 CalcSpotLight(vec3 light2Pixel, vec3 vertexToEye, vec3 normal)
{
    vec4 color = CalcPointLight(light2Pixel, vertexToEye, normal);
    light2Pixel = normalize(light2Pixel);
    float spotFactor = dot(light2Pixel, u_lightDirection);
    float spotAtten = clamp((spotFactor - u_lightCutOff) / (1.0 - u_lightCutOff), 0.0, 1.0);
    return color * spotAtten;
}

vec4 CalcTotalLight(vec3 world2light, vec3 vertexToEye, vec3 normal)
{
    #if defined(COMPILEFS) && (defined(SHADOWMAP) || defined(CUBESHADOWMAP))
        #if defined(HAS_DIRECTIONAL_LIGHT)
            return CalcShadowFactor(world2light) * CalcLight(u_lightDirection, vertexToEye, normal);
        #elif defined(HAS_POINT_LIGHT)
            return CalcShadowFactor(world2light) * CalcPointLight(world2light, vertexToEye, normal);
        #elif defined(HAS_SPOT_LIGHT)
            return CalcShadowFactor(world2light) * CalcSpotLight(world2light, vertexToEye, normal);
        #else 
            return vec4(1.0);        
        #endif
    #else
        #if defined(HAS_DIRECTIONAL_LIGHT)
            return CalcLight(u_lightDirection, vertexToEye, normal);
        #elif defined(HAS_POINT_LIGHT)
            return CalcPointLight(world2light, vertexToEye, normal);
        #elif defined(HAS_SPOT_LIGHT)
            return CalcSpotLight(world2light, vertexToEye, normal);
        #else 
            return vec4(1.0);        
        #endif
    #endif
}
