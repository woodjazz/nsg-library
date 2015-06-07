//Remember to rebuild with CMake if this file changes

#if defined(COMPILEFS)

    #if defined(AMBIENT_PASS)

        vec4 GetAmbientIntensity()
        {
            #if !defined(AOMAP0) && !defined(AOMAP1) && !defined(LIGHTMAP0) && !defined(LIGHTMAP1)

                #if defined(UNLIT)

                    vec4 intensity = u_material.color;

                #else

                    vec4 intensity = u_sceneAmbientColor * u_material.ambient;

                #endif

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
                return GetAmbientIntensity() * texture2D(u_texture0, v_texcoord0);
            #else
                return GetAmbientIntensity();
            #endif
        }

    #endif

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
    vec4 CalcPointLight(BaseLight base, vec3 lightDirection, vec3 vertexToEye, vec3 normal)
    {
        float lightDist = length(lightDirection * u_lightInvRange);
        lightDirection = normalize(lightDirection);
        vec4 color = CalcLight(base, vertexToEye, lightDirection, normal);
        float atten = clamp(1.0 - lightDist * lightDist, 0.0, 1.0);
        return color * atten;
    }
    #endif

    #ifdef HAS_SPOT_LIGHT
    vec4 CalcSpotLight(BaseLight base, vec3 light2Pixel, vec3 lightDirection, vec3 vertexToEye, vec3 normal, float cutOff)
    {
        lightDirection = normalize(lightDirection);
        vec4 color = CalcPointLight(base, light2Pixel, vertexToEye, normal);
        light2Pixel = normalize(light2Pixel);
        float spotFactor = dot(light2Pixel, lightDirection);
        float spotAtten = clamp((spotFactor - cutOff) / (1.0 - cutOff), 0.0, 1.0);
        return color * spotAtten;
    }
    #endif

    #if defined(COMPILEFS)

        #if defined(CUBESHADOWMAP)

            vec3 FixCubeLookup(vec3 v) 
            {
                // To eliminate the edge seams
                // Since the extension ARB_seamless_cube_map is not always available.
                // From http://the-witness.net/news/2012/02/seamless-cube-map-filtering 
                float cube_size = 1.0/u_shadowMapInvSize;
                float M = max(max(abs(v.x), abs(v.y)), abs(v.z)); 
                float scale = (cube_size - 1.0) / cube_size; 
                if (abs(v.x) != M) v.x *= scale; 
                if (abs(v.y) != M) v.y *= scale; 
                if (abs(v.z) != M) v.z *= scale; 
                return v; 
            }

            vec4 CalcShadowCubeFactor()
            {
                float lengthLightDirection = length(v_lightDirection);
                float totalError = u_shadowBias * lengthLightDirection;
                float sampledDistance = DecodeColor2Depth(textureCube(u_texture5, FixCubeLookup(v_lightDirection))) / u_lightInvRange;
                return sampledDistance + totalError < lengthLightDirection ? u_shadowColor : vec4(1.0);
            }

        #elif defined(SHADOWMAP)

            vec4 CalcShadowFactor()
            {
                float lengthLightDirection = length(v_lightDirection);
                float totalError = u_shadowBias * lengthLightDirection;
                float lightToPixelDistance = clamp(lengthLightDirection * u_lightInvRange, 0.0, 1.0);
                vec4 coords = v_shadowClipPos / v_shadowClipPos.w; // Normalize from -w..w to -1..1
                coords = 0.5 * coords + 0.5; // Normalize from -1..1 to 0..1
                // Take four samples and average them
                float sampledDistance = DecodeColor2Depth(texture2D(u_texture5, coords.xy));
                sampledDistance += DecodeColor2Depth(texture2D(u_texture5, coords.xy + vec2(u_shadowMapInvSize, 0.0)));
                sampledDistance += DecodeColor2Depth(texture2D(u_texture5, coords.xy + vec2(0.0, u_shadowMapInvSize)));
                sampledDistance += DecodeColor2Depth(texture2D(u_texture5, coords.xy + vec2(u_shadowMapInvSize)));
                sampledDistance *= 0.25;
                return sampledDistance + totalError < lightToPixelDistance ? u_shadowColor : vec4(1.0);
            }

        #endif

        vec4 CalcTotalLight(vec3 vertexToEye, vec3 normal)
        {
            #if defined(HAS_DIRECTIONAL_LIGHT)

                #if defined(SHADOWMAP)
                    return CalcShadowFactor() * CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);
                #else
                    return CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);
                #endif                    

            #elif defined(HAS_POINT_LIGHT)

                #if defined(CUBESHADOWMAP)
                    return CalcShadowCubeFactor() * CalcPointLight(u_pointLight.base, v_lightDirection, vertexToEye, normal);
                #else
                    return CalcPointLight(u_pointLight.base, v_lightDirection, vertexToEye, normal);
                #endif

            #elif defined(HAS_SPOT_LIGHT)

                #if defined(SHADOWMAP)
                    return CalcShadowFactor() * CalcSpotLight(u_spotLight.base, v_lightDirection, u_spotLight.direction, vertexToEye, normal, u_spotLight.cutOff);
                #else
                    return CalcSpotLight(u_spotLight.base, v_lightDirection, u_spotLight.direction, vertexToEye, normal, u_spotLight.cutOff);
                #endif

            #endif
        }

    #elif defined(COMPILEVS)

        vec4 CalcTotalLight(vec3 worldPos, vec3 vertexToEye, vec3 normal)
        {
            #if defined(HAS_DIRECTIONAL_LIGHT)

                return CalcDirectionalLight(u_directionalLight.base, u_directionalLight.direction, vertexToEye, normal);

            #elif defined(HAS_POINT_LIGHT)

                return CalcPointLight(u_pointLight.base, worldPos - u_pointLight.position, vertexToEye, normal);

            #elif defined(HAS_SPOT_LIGHT)

                return CalcSpotLight(u_spotLight.base, worldPos - u_spotLight.position, u_spotLight.direction, vertexToEye, normal, u_spotLight.cutOff);

            #endif
        }

    #endif    

#endif