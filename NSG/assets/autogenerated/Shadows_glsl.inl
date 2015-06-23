#pragma once
namespace NSG
{
static const char* SHADOWS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(SHADOWMAP) || defined(CUBESHADOWMAP)\n"\
"    float GetShadowMapInvSize()\n"\
"    {\n"\
"        return u_shadowMapInvSize[GetSplit()];\n"\
"    }\n"\
"#endif\n"\
"#if defined(CUBESHADOWMAP)\n"\
"    vec3 FixCubeLookup(vec3 v) \n"\
"    {\n"\
"        // To eliminate the edge seams\n"\
"        // Since the extension ARB_seamless_cube_map is not always available.\n"\
"        // From http://the-witness.net/news/2012/02/seamless-cube-map-filtering \n"\
"        float cube_size = 1.0/GetShadowMapInvSize();\n"\
"        float M = max(max(abs(v.x), abs(v.y)), abs(v.z)); \n"\
"        float scale = (cube_size - 1.0) / cube_size; \n"\
"        if (abs(v.x) != M) v.x *= scale; \n"\
"        if (abs(v.y) != M) v.y *= scale; \n"\
"        if (abs(v.z) != M) v.z *= scale; \n"\
"        return v; \n"\
"    }\n"\
"#if 0\n"\
"    vec2 GetShadowOffsets(vec3 N, vec3 L) \n"\
"    { \n"\
"        //Being N the surface normal and L the light direction\n"\
"        float cos_alpha = saturate(dot(N, L)); \n"\
"        float offset_scale_N = sqrt(1 - cos_alpha*cos_alpha); \n"\
"        // sin(acos(L\n"\
"N)) \n"\
"        float offset_scale_L = offset_scale_N / cos_alpha; \n"\
"        // tan(acos(L\n"\
"N)) \n"\
"        return vec2(offset_scale_N, min(2.0, offset_scale_L)); \n"\
"    }\n"\
"#endif\n"\
"    vec4 CalcShadowFactor(vec3 world2light)\n"\
"    {\n"\
"        float sampledDistance = DecodeColor2Depth(textureCube(u_texture3, FixCubeLookup(world2light)));\n"\
"        return sampledDistance / GetLightInvRange() < length(world2light) - u_shadowBias ? u_shadowColor : vec4(1.0);\n"\
"    }\n"\
"#elif defined(SHADOWMAP)\n"\
"    vec4 GetShadowClipPos(vec4 worldPos)\n"\
"    {\n"\
"        return u_lightViewProjection[GetSplit()] * worldPos;\n"\
"    }\n"\
"    vec4 GetTexture2DFromShadowMap(vec2 coord)\n"\
"    {\n"\
"        int split = GetSplit();\n"\
"        if(split == 0)\n"\
"            return texture2D(u_texture3, coord);\n"\
"        else if(split == 1)\n"\
"            return texture2D(u_texture4, coord);\n"\
"        else if(split == 2)\n"\
"            return texture2D(u_texture5, coord);\n"\
"        else\n"\
"            return texture2D(u_texture6, coord);\n"\
"    }\n"\
"    vec4 CalcShadowFactor(vec3 world2light)\n"\
"    {\n"\
"        const vec4 White = vec4(1.0);\n"\
"        const vec4 Red = vec4(1.0, 0.0, 0.0, 1.0);\n"\
"        const vec4 Green = vec4(0.0, 1.0, 0.0, 1.0);\n"\
"        const vec4 Blue = vec4(0.0, 0.0, 1.0, 1.0);\n"\
"        const vec4 RedOrange = vec4(1.0, 69.0/255.0, 0.0, 1.0);\n"\
"        vec4 shadowClipPos = GetShadowClipPos(vec4(v_worldPos, 1.0));\n"\
"        vec4 coords = shadowClipPos / shadowClipPos.w; // Normalize from -w..w to -1..1\n"\
"        //if(coords.x >= 1.0 || coords.x <= -1.0 || coords.y >= 1.0 || coords.y <= -1.0)\n"\
"        //    return vec4(1.0,0.0,0.0,1.0);\n"\
"        //if(coords.z >= 1.0 || coords.z <= -1.0)\n"\
"        //    return vec4(0.0,1.0,0.0,1.0);\n"\
"        coords = 0.5 * coords + 0.5; // Normalize from -1..1 to 0..1\n"\
"        // Take four samples and average them\n"\
"        float sampledDistance = DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy));\n"\
"        float shadowMapInvSize = GetShadowMapInvSize();\n"\
"        sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(shadowMapInvSize, 0.0)));\n"\
"        sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(0.0, shadowMapInvSize)));\n"\
"        sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(shadowMapInvSize)));\n"\
"        sampledDistance *= 0.25;\n"\
"        bool inShadow = sampledDistance / GetLightInvRange() < length(world2light) - u_shadowBias;\n"\
"        #ifdef COLOR_SPLITS\n"\
"            int split = GetSplit();\n"\
"            if(split == 0)\n"\
"                return inShadow ? Red : White;\n"\
"            else if(split == 1)\n"\
"                return inShadow ? Green : White;\n"\
"            else if(split == 2)\n"\
"                return inShadow ? Blue : White;\n"\
"            else\n"\
"                return inShadow ? RedOrange : White;\n"\
"        #else\n"\
"            return inShadow ? u_shadowColor : White;\n"\
"        #endif\n"\
"    }\n"\
"#endif\n"\
;
}
