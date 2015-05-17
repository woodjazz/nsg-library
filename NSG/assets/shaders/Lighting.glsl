//Remember to rebuild with CMake if this file changes

#if defined(COMPILEFS)
vec4 GetAmbientIntensity()
{
    #if defined(AOMAP0)
        #if defined(AOMAP_CHANNELS1)
            vec4 intensity = vec4(texture2D(u_texture4, v_texcoord0).a);
        #else
            vec4 intensity = texture2D(u_texture4, v_texcoord0);
        #endif
    #elif defined(AOMAP1)
        #if defined(AOMAP_CHANNELS1)
            vec4 intensity = vec4(texture2D(u_texture4, v_texcoord1).a);
        #else
            vec4 intensity = texture2D(u_texture4, v_texcoord1);
        #endif
    #else
        vec4 intensity = u_sceneAmbientColor * u_material.ambient;
    #endif

    #if defined(LIGHTMAP0)
        #if defined(LIGHTMAP_CHANNELS1)
            intensity *= vec4(texture2D(u_texture2, v_texcoord0).a);
        #else
            intensity *= texture2D(u_texture2, v_texcoord0);
        #endif
    #elif defined(LIGHTMAP1)
        #if defined(LIGHTMAP_CHANNELS1)
            intensity *= vec4(texture2D(u_texture2, v_texcoord1).a);
        #else
            intensity *= texture2D(u_texture2, v_texcoord1);
        #endif
    #endif

    return intensity;
}

vec4 GetAmbientLight()
{
    #ifdef DIFFUSEMAP
        vec4 diffColor = u_material.diffuse * texture2D(u_texture0, v_texcoord0);
    #else
        vec4 diffColor = u_material.diffuse;
    #endif

    return GetAmbientIntensity() * diffColor;
}
#endif

#if defined(HAS_DIRECTIONAL_LIGHT) || defined(HAS_POINT_LIGHT) || defined(HAS_SPOT_LIGHT)

vec4 CalcLight(BaseLight base, vec3 vertexToEye, vec3 lightDirection, vec3 normal)
{
    float diffuseFactor = clamp(dot(normal, -lightDirection), 0.0, 1.0);
    vec4 color = diffuseFactor * base.diffuse * u_material.diffuse;
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
vec4 CalcPointLight(BaseLight base, vec3 lightDirection, float invRange, vec3 vertexToEye, vec3 normal)
{
    float lightDist = length(lightDirection * invRange);
    lightDirection = normalize(lightDirection);
    vec4 color = CalcLight(base, vertexToEye, lightDirection, normal);
    float atten = clamp(1.0 - lightDist * lightDist, 0.0, 1.0);
    return color * atten;
}
#endif

#ifdef HAS_SPOT_LIGHT
vec4 CalcSpotLight(BaseLight base, vec3 light2Pixel, vec3 lightDirection, float invRange, vec3 vertexToEye, vec3 normal, float cutOff)
{
    lightDirection = normalize(lightDirection);
    vec4 color = CalcPointLight(base, light2Pixel, invRange, vertexToEye, normal);
    light2Pixel = normalize(light2Pixel);
    float spotFactor = dot(light2Pixel, lightDirection);
    float spotAtten = clamp((spotFactor - cutOff) / (1.0 - cutOff), 0.0, 1.0);
    return color * spotAtten;
}
#endif

vec4 CalcFSTotalLight(vec3 vertexToEye, vec3 normal)
{
    #if defined(HAS_DIRECTIONAL_LIGHT)

        return u_sceneAmbientColor + CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);

    #elif defined(HAS_POINT_LIGHT)

        return u_sceneAmbientColor + CalcPointLight(u_pointLight.base, v_lightDirection, u_pointLight.invRange, vertexToEye, normal);

    #elif defined(HAS_SPOT_LIGHT)

        return u_sceneAmbientColor + CalcSpotLight(u_spotLight.base, v_lightDirection, u_spotLight.direction, u_spotLight.invRange, vertexToEye, normal, u_spotLight.cutOff);

    #endif
}

vec4 CalcVSTotalLight(vec3 worldPos, vec3 vertexToEye, vec3 normal)
{
    #if defined(HAS_DIRECTIONAL_LIGHT)

        return u_sceneAmbientColor + CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);

    #elif defined(HAS_POINT_LIGHT)

        return u_sceneAmbientColor + CalcPointLight(u_pointLight.base, worldPos - u_pointLight.position, u_pointLight.invRange, vertexToEye, normal);

    #elif defined(HAS_SPOT_LIGHT)

        return u_sceneAmbientColor + CalcSpotLight(u_spotLight.base, worldPos - u_spotLight.position, u_spotLight.direction, u_spotLight.invRange, vertexToEye, normal, u_spotLight.cutOff);

    #endif
}
#endif
