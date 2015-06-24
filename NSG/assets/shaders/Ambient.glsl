//Remember to rebuild with CMake if this file changes

#if defined(COMPILEFS)
    #if defined(AMBIENT_PASS)
        vec4 GetAmbientIntensity()
        {
            #if !defined(AOMAP0) && !defined(AOMAP1) && !defined(LIGHTMAP0) && !defined(LIGHTMAP1)
                vec4 intensity = u_sceneAmbientColor * u_material.ambient;
            #else
                #if defined(AOMAP0)
                    #if defined(AOMAP_CHANNELS1)
                        vec4 intensity = vec4(texture2D(u_texture1, v_texcoord0).a);
                    #else
                        vec4 intensity = texture2D(u_texture1, v_texcoord0);
                    #endif
                #elif defined(AOMAP1)
                    #if defined(AOMAP_CHANNELS1)
                        vec4 intensity = vec4(texture2D(u_texture1, v_texcoord1).a);
                    #else
                        vec4 intensity = texture2D(u_texture1, v_texcoord1);
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
                #if defined(UNLIT)
                    return texture2D(u_texture0, v_texcoord0);
                #else
                    return u_material.diffuse.a * GetAmbientIntensity() * texture2D(u_texture0, v_texcoord0);
                #endif
            #else
                #if defined(UNLIT)
                    return u_material.diffuse;
                #else
                    return u_material.diffuse.a * GetAmbientIntensity();
                #endif
            #endif
        }
    #endif
#endif
