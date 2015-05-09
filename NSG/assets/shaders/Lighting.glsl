//Remember to rebuild with CMake if this file changes
#if defined(HAS_DIRECTIONAL_LIGHT) || defined(HAS_POINT_LIGHT) || defined(HAS_SPOT_LIGHT)

vec4 CalcLight(BaseLight base, vec3 vertexToEye, vec3 lightDirection, vec3 normal)
{
    #if defined(AOMAP) && defined(COMPILEFS)
    vec4 color = texture2D(u_texture4, v_texcoord1);
    #else
    vec4 color = u_material.ambient * base.ambient;
    #endif

    float diffuseFactor = clamp(dot(normal, -lightDirection), 0.0, 1.0);
    color = max(color, diffuseFactor * base.diffuse * u_material.diffuse);
    vec3 lightReflect = normalize(reflect(lightDirection, normal));
    float specularFactor = clamp(dot(vertexToEye, lightReflect), 0.0, 1.0);
    specularFactor = pow(specularFactor, u_material.shininess);
    #if defined SPECULARMAP && defined(COMPILEFS)
    color += specularFactor * base.specular * u_material.specular * texture2D(u_texture3, v_texcoord0);
    #else
    color += specularFactor * base.specular * u_material.specular;
    #endif

    return color;
}

#ifdef HAS_DIRECTIONAL_LIGHT
vec4 CalcDirectionalLight(BaseLight base, vec3 lightDirection, vec3 vertexToEye, vec3 normal)
{
    return CalcLight(base, vertexToEye, lightDirection, normal);
}
#endif

#if defined(HAS_POINT_LIGHT) || defined(HAS_SPOT_LIGHT)
vec4 CalcPointLight(BaseLight base, vec3 lightDirection, Attenuation attenuation, vec3 vertexToEye, vec3 normal)
{
    float distance = length(lightDirection);
    lightDirection = normalize(lightDirection);
    vec4 color = CalcLight(base, vertexToEye, lightDirection, normal);
    float attenuationFactor =  attenuation.constant + attenuation.linear * distance + attenuation.quadratic * distance * distance;
    return color / attenuationFactor;
}
#endif

#ifdef HAS_SPOT_LIGHT
vec4 CalcSpotLight(BaseLight base, vec3 light2Pixel, vec3 lightDirection, Attenuation attenuation, vec3 vertexToEye, vec3 normal, float cutOff)
{
    lightDirection = normalize(lightDirection);
    light2Pixel = normalize(light2Pixel);
    float spotFactor = dot(light2Pixel, lightDirection);

    if (spotFactor > cutOff)
    {
        vec4 color = CalcPointLight(base, light2Pixel, attenuation, vertexToEye, normal);
        return color * (1.0 - (1.0 - spotFactor) * 1.0 / (1.0 - cutOff));
    }
    else
    {
        return vec4(0.0);
    }
}
#endif

vec4 CalcFSTotalLight(vec3 vertexToEye, vec3 normal)
{
    #if defined(HAS_DIRECTIONAL_LIGHT)

        return u_sceneAmbientColor + CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);

    #elif defined(HAS_POINT_LIGHT)

        return u_sceneAmbientColor + CalcPointLight(u_pointLight.base, v_lightDirection, u_pointLight.atten, vertexToEye, normal);

    #elif defined(HAS_SPOT_LIGHT)

        return u_sceneAmbientColor + CalcSpotLight(u_spotLight.base, v_light2Pixel, u_spotLight.direction, u_spotLight.atten, vertexToEye, normal, u_spotLight.cutOff);

    #endif
}

vec4 CalcVSTotalLight(vec3 worldPos, vec3 vertexToEye, vec3 normal)
{
    #if defined(HAS_DIRECTIONAL_LIGHT)

        return u_sceneAmbientColor + CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);

    #elif defined(HAS_POINT_LIGHT)

        return u_sceneAmbientColor + CalcPointLight(u_pointLight.base, worldPos - u_pointLight.position, u_pointLight.atten, vertexToEye, normal);

    #elif defined(HAS_SPOT_LIGHT)

        return u_sceneAmbientColor + CalcSpotLight(u_spotLight.base, worldPos - u_spotLight.position, u_spotLight.direction, u_spotLight.atten, vertexToEye, normal, u_spotLight.cutOff);

    #endif
}
#endif
