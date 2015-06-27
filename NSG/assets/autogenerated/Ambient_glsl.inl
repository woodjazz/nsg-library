#pragma once
namespace NSG
{
static const char* AMBIENT_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(COMPILEFS)\n"\
"    #if defined(AMBIENT)\n"\
"        vec4 GetAmbientIntensity()\n"\
"        {\n"\
"            #if !defined(AOMAP0) && !defined(AOMAP1) && !defined(LIGHTMAP0) && !defined(LIGHTMAP1)\n"\
"                vec4 intensity = u_sceneAmbientColor * u_material.ambient;\n"\
"            #else\n"\
"                #if defined(AOMAP0)\n"\
"                    #if defined(AOMAP_CHANNELS1)\n"\
"                        vec4 intensity = vec4(texture2D(u_texture1, v_texcoord0).a);\n"\
"                    #else\n"\
"                        vec4 intensity = texture2D(u_texture1, v_texcoord0);\n"\
"                    #endif\n"\
"                #elif defined(AOMAP1)\n"\
"                    #if defined(AOMAP_CHANNELS1)\n"\
"                        vec4 intensity = vec4(texture2D(u_texture1, v_texcoord1).a);\n"\
"                    #else\n"\
"                        vec4 intensity = texture2D(u_texture1, v_texcoord1);\n"\
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
"                vec4 diffuseMap = texture2D(u_texture0, v_texcoord0);\n"\
"                return GetAmbientIntensity() * vec4(diffuseMap.rgb, diffuseMap.a + u_material.diffuse.a);\n"\
"            #else\n"\
"                return u_material.diffuse.a * GetAmbientIntensity();\n"\
"            #endif\n"\
"        }\n"\
"    #endif\n"\
"#endif\n"\
;
}
