//Remember to rebuild with CMake if this file changes

#if defined(COMPILEFS)

    vec4 GetAmbientIntensity()
    {
        #if !defined(AOMAP0) && !defined(AOMAP1) && !defined(LIGHTMAP0) && !defined(LIGHTMAP1)

            vec4 intensity = u_sceneAmbientColor * u_material.ambient;

        #else

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

                vec4 intensity = vec4(1.0);

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

    #if defined(COMPILEFS)

        #if defined(CUBESHADOWMAP)

            float CalcShadowCubeFactor(vec3 lightDirection)
            {
                float lightToPixelDistance = DecodeColor2Depth(textureCube(u_texture5, lightDirection).xyz) / u_pointLight.invRange;
                float distance = length(lightDirection);
                return lightToPixelDistance < distance ? 0.0 : 1.0;
            }

        #elif defined(SHADOWMAP)

            float CalcShadowFactor()
            {
                vec3 encodedDepth = texture2DProj(u_texture5, v_lightSpacePos).xyz;
                return DecodeColor2Depth(encodedDepth) < v_lightSpacePos.z ? 0.0 : 1.0;
            }

        #endif

        vec4 CalcTotalLight(vec3 vertexToEye, vec3 normal)
        {
            #if defined(HAS_DIRECTIONAL_LIGHT)

                #if 0//defined(SHADOWMAP)
                    return CalcShadowFactor() * CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);
                #else
                    return CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);
                #endif                    

            #elif defined(HAS_POINT_LIGHT)

                #if 0//defined(CUBESHADOWMAP)
                    return CalcShadowCubeFactor(v_lightDirection) * CalcPointLight(u_pointLight.base, v_lightDirection, u_pointLight.invRange, vertexToEye, normal);
                #else
                    return CalcPointLight(u_pointLight.base, v_lightDirection, u_pointLight.invRange, vertexToEye, normal);
                #endif

            #elif defined(HAS_SPOT_LIGHT)

                #if defined(SHADOWMAP)
                    return CalcShadowFactor() * CalcSpotLight(u_spotLight.base, v_lightDirection, u_spotLight.direction, u_spotLight.invRange, vertexToEye, normal, u_spotLight.cutOff);
                #else
                    return CalcSpotLight(u_spotLight.base, v_lightDirection, u_spotLight.direction, u_spotLight.invRange, vertexToEye, normal, u_spotLight.cutOff);
                #endif

            #endif
        }

    #elif defined(COMPILEVS)

        vec4 CalcTotalLight(vec3 worldPos, vec3 vertexToEye, vec3 normal)
        {
            #if defined(HAS_DIRECTIONAL_LIGHT)

                return CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);

            #elif defined(HAS_POINT_LIGHT)

                return CalcPointLight(u_pointLight.base, worldPos - u_pointLight.position, u_pointLight.invRange, vertexToEye, normal);

            #elif defined(HAS_SPOT_LIGHT)

                return CalcSpotLight(u_spotLight.base, worldPos - u_spotLight.position, u_spotLight.direction, u_spotLight.invRange, vertexToEye, normal, u_spotLight.cutOff);

            #endif
        }

    #endif    

#endif