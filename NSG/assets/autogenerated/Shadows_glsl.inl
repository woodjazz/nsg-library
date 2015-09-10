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
"        //return sampledDistance / u_lightInvRange < length(world2light) - u_shadowBias ? u_shadowColor : vec4(1.0);\n"\
"        return sampledDistance / u_lightInvRange < length(world2light) ? u_shadowColor : vec4(1.0);\n"\
"    }\n"\
"#elif defined(SHADOWMAP)\n"\
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
"#if 0\n"\
"    // Based on http://jcgt.org/published/0003/04/08/paper-lowres.pdf\n"\
"    float CalcAdaptativeDepthBias(int split, vec4 smTexCoord)\n"\
"    {\n"\
"        // Shadow map resolution\n"\
"        float smBufferRes = 1.0/u_shadowMapInvSize[split];\n"\
"        // Locate corresponding light space shadow map grid center\n"\
"        vec2  index = floor( vec2( smTexCoord.xy * smBufferRes) );\n"\
"        float delta = 1.0 / smBufferRes;\n"\
"        // Normalized coordinate in [0,1]\n"\
"        vec2  nlsGridCenter = delta*(index + vec2(0.5)); // Normalized eye space grid center --- [0,1]\n"\
"        // Unnormalized coordinate in [-lightLeft,lightLeft]\n"\
"        vec2 lsGridCenter = u_lightViewBound*( 2.0*nlsGridCenter - vec2(1.0) );\n"\
"        \n"\
"        /** Define light ray **/\n"\
"        // Light ray direction in light space\n"\
"        vec3 lsGridLineDir = normalize( vec3(lsGridCenter, -lightNear) ); // Light space grid line direction    \n"\
"        \n"\
"        /** Plane ray intersection **/\n"\
"        // Locate the potential occluder for the shading fragment\n"\
"        float ls_t_hit = dot(n, lsFragPos.xyz) / dot(n, lsGridLineDir);\n"\
"        vec3  ls_hit_p = ls_t_hit * lsGridLineDir;\n"\
"        \n"\
"        /** Compute Adaptive Epsilon **/\n"\
"        // Normalized depth value in shadow map\n"\
"        float SMDepth = texture( shadowTex, smTexCoord.xy );\n"\
"        // A and B are computed bnased on light near and far planes.\n"\
"        // They can be retrieved directly from light projection matrix\n"\
"        float A = lightProj[2][2];\n"\
"        float B = lightProj[3][2];    \n"\
"        float adaptiveDepthBias = 0.5*pow(1.0 - A - 2.0*SMDepth, 2)*constantBias / B; \n"\
"        \n"\
"        // Use the intersection point as new look up point\n"\
"        vec4 lsPotentialoccluder = lightProj * vec4(ls_hit_p, 1.0);\n"\
"        lsPotentialoccluder      = lsPotentialoccluder/lsPotentialoccluder.w;\n"\
"        lsPotentialoccluder      = 0.5 * lsPotentialoccluder + vec4(0.5, 0.5, 0.5, 0.0);\n"\
"       \n"\
"        float actualDepth = min(lsPotentialoccluder.z, smTexCoord.z);\n"\
"        float actualBias  = adaptiveDepthBias;\n"\
"            \n"\
"        // Constant depth bias VS adaptive depth bias\n"\
"        actualDepth = adaptiveFlag != 0 ? actualDepth : smTexCoord.z;\n"\
"        actualBias  = adaptiveFlag != 0 ? actualBias  : zBias;    \n"\
"    }\n"\
"#endif\n"\
"    vec4 CalcShadowFactor()\n"\
"    {\n"\
"        const vec4 White = vec4(1.0);\n"\
"        // Transform from eye-space to shadow map texture coordinates\n"\
"        vec4 coords = GetShadowClipPos(vec4(v_worldPos, 1.0));\n"\
"        coords /= coords.w; // Normalize from -w..w to -1..1\n"\
"        coords  = 0.5 * coords + vec4(0.5, 0.5, 0.5, 0.0); // Normalize from -1..1 to 0..1\n"\
"        // If region is out of shadowcam frustum then not in shadow\n"\
"        if(clamp(coords.xyz, 0.0, 1.0) != coords.xyz)\n"\
"            return White;\n"\
"        // Take four samples and average them\n"\
"        float sampledDistance = DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy));\n"\
"        #if 0\n"\
"        float shadowMapInvSize = GetShadowMapInvSize();\n"\
"        sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(shadowMapInvSize, 0.0)));\n"\
"        sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(0.0, shadowMapInvSize)));\n"\
"        sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(shadowMapInvSize)));\n"\
"        sampledDistance *= 0.25;\n"\
"        #endif\n"\
"        bool inShadow = sampledDistance < coords.z;// - u_shadowBias;\n"\
"        #ifdef COLOR_SPLITS\n"\
"            const vec4 Red = vec4(1.0, 0.0, 0.0, 1.0);\n"\
"            const vec4 Green = vec4(0.0, 1.0, 0.0, 1.0);\n"\
"            const vec4 Blue = vec4(0.0, 0.0, 1.0, 1.0);\n"\
"            const vec4 RedOrange = vec4(1.0, 69.0/255.0, 0.0, 1.0);\n"\
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
