//Remember to rebuild with CMake if this file changes

#if defined(CUBESHADOWMAP)

    vec4 CalcShadowFactor(vec3 world2light)
    {
        float sampledDistance = DecodeColor2Depth(textureCube(u_texture3, FixCubeLookup(world2light)));
        return sampledDistance / u_lightInvRange < length(world2light) - u_shadowBias ? u_shadowColor : vec4(1.0);
    }

#elif defined(SHADOWMAP)

    vec4 CalcShadowFactor()
    {
        const vec4 White = vec4(1.0);
        
        vec4 coords = GetTextureCoords(vec4(v_worldPos, 1.0));
        
        //If region is out of shadowcam frustum then not in shadow (White)
        if(clamp(coords.xyz, 0.0, 1.0).xyz != coords.xyz)
            return White;//vec4(1.0, 0.0, 1.0, 1.0);

        float sampledDistance = DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy));

        #ifndef IS_TARGET_MOBILE
            // Take four samples and average them
            float shadowMapInvSize = GetShadowMapInvSize();
            sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(shadowMapInvSize, 0.0)));
            sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(0.0, shadowMapInvSize)));
            sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(shadowMapInvSize)));
            sampledDistance *= 0.25;
        #endif

        bool inShadow = sampledDistance < coords.z - u_shadowBias;

        #ifdef COLOR_SPLITS
            return inShadow ? GetSplitColor() : White;
        #else
            return inShadow ? u_shadowColor : White;
        #endif
    }


#endif
