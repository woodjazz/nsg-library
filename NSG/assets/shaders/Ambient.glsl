//Remember to rebuild with CMake if this file changes

#if defined(COMPILEFS)
    #if defined(AMBIENT) || defined(UNLIT)
        vec4 GetAmbientIntensity()
        {
            #if !defined(AOMAP0) && !defined(AOMAP1) && !defined(LIGHTMAP0) && !defined(LIGHTMAP1)
                #if defined(AMBIENT)
                    vec4 intensity = vec4(u_sceneAmbientColor, 1.0) * u_material.ambientIntensity;
                #else
                    vec4 intensity = vec4(1.0);
                #endif
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
    #endif
#endif
