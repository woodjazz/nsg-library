#pragma once
namespace NSG
{
static const char* SHADOWS_GLSL = \
"//Remember to rebuild with CMake if this file changes\n"\
"#if defined(CUBESHADOWMAP)\n"\
"    vec4 CalcShadowFactor(vec3 world2light)\n"\
"    {\n"\
"        float sampledDistance = DecodeColor2Depth(textureCube(u_texture3, FixCubeLookup(world2light)));\n"\
"        return sampledDistance / u_lightInvRange < length(world2light) - u_shadowBias ? u_shadowColor : vec4(1.0);\n"\
"    }\n"\
"#elif defined(SHADOWMAP)\n"\
"    vec4 CalcShadowFactor(vec3 world2light)\n"\
"    {\n"\
"        const vec4 White = vec4(1.0);\n"\
"        \n"\
"        vec4 coords = GetTextureCoords(vec4(v_worldPos, 1.0));\n"\
"        \n"\
"        //If region is out of shadowcam frustum then not in shadow (White)\n"\
"        if(clamp(coords.xyz, 0.0, 1.0).xy != coords.xy)\n"\
"            return White;//vec4(1.0, 0.0, 1.0, 1.0);\n"\
"        float sampledDistance = DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy));\n"\
"        #ifndef IS_TARGET_MOBILE\n"\
"            // Take four samples and average them\n"\
"            float shadowMapInvSize = GetShadowMapInvSize();\n"\
"            sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(shadowMapInvSize, 0.0)));\n"\
"            sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(0.0, shadowMapInvSize)));\n"\
"            sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(shadowMapInvSize)));\n"\
"            sampledDistance *= 0.25;\n"\
"        #endif\n"\
"        #ifdef HAS_DIRECTIONAL_LIGHT\n"\
"            bool inShadow = sampledDistance / GetShadowCamInvRange() < length(world2light) - u_shadowBias;\n"\
"            //bool inShadow = sampledDistance < coords.z - u_shadowBias;\n"\
"        #else\n"\
"            bool inShadow = sampledDistance / u_lightInvRange < length(world2light) - u_shadowBias;\n"\
"        #endif\n"\
"        #ifdef COLOR_SPLITS\n"\
"            return inShadow ? GetSplitColor() : White;\n"\
"        #else\n"\
"            return inShadow ? u_shadowColor : White;\n"\
"        #endif\n"\
"    }\n"\
"#endif\n"\
;
}
