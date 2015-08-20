//Remember to rebuild with CMake if this file changes

#if defined(SHADOWMAP) || defined(CUBESHADOWMAP)

    float GetShadowMapInvSize()
    {
        return u_shadowMapInvSize[GetSplit()];
    }

#endif

#if defined(CUBESHADOWMAP)

    vec3 FixCubeLookup(vec3 v) 
    {
        // To eliminate the edge seams
        // Since the extension ARB_seamless_cube_map is not always available.
        // From http://the-witness.net/news/2012/02/seamless-cube-map-filtering 
        float cube_size = 1.0/GetShadowMapInvSize();
        float M = max(max(abs(v.x), abs(v.y)), abs(v.z)); 
        float scale = (cube_size - 1.0) / cube_size; 
        if (abs(v.x) != M) v.x *= scale; 
        if (abs(v.y) != M) v.y *= scale; 
        if (abs(v.z) != M) v.z *= scale; 
        return v; 
    }

#if 0
    vec2 GetShadowOffsets(vec3 N, vec3 L) 
    { 
        //Being N the surface normal and L the light direction

        float cos_alpha = saturate(dot(N, L)); 
        float offset_scale_N = sqrt(1 - cos_alpha*cos_alpha); 
        // sin(acos(L·N)) 
        float offset_scale_L = offset_scale_N / cos_alpha; 
        // tan(acos(L·N)) 
        return vec2(offset_scale_N, min(2.0, offset_scale_L)); 
    }
#endif

    vec4 CalcShadowFactor(vec3 world2light)
    {
        float sampledDistance = DecodeColor2Depth(textureCube(u_texture3, FixCubeLookup(world2light)));
        return sampledDistance / GetLightInvRange() < length(world2light) - u_shadowBias ? u_shadowColor : vec4(1.0);
    }

#elif defined(SHADOWMAP)

    vec4 GetShadowClipPos(vec4 worldPos)
    {
        return u_lightViewProjection[GetSplit()] * worldPos;
    }

    vec4 GetTexture2DFromShadowMap(vec2 coord)
    {
        int split = GetSplit();
        if(split == 0)
            return texture2D(u_texture3, coord);
        else if(split == 1)
            return texture2D(u_texture4, coord);
        else if(split == 2)
            return texture2D(u_texture5, coord);
        else
            return texture2D(u_texture6, coord);
    }

    vec4 CalcShadowFactor(vec3 world2light)
    {
        const vec4 White = vec4(1.0);
        vec4 shadowClipPos = GetShadowClipPos(vec4(v_worldPos, 1.0));
        vec4 coords = shadowClipPos / shadowClipPos.w; // Normalize from -w..w to -1..1
        if(coords.x >= 1.0 || coords.x <= -1.0 || coords.y >= 1.0 || coords.y <= -1.0)
            return White;
        if(coords.z >= 1.0 || coords.z <= -1.0)
            return White;
        coords = 0.5 * coords + 0.5; // Normalize from -1..1 to 0..1
        // Take four samples and average them
        float sampledDistance = DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy));
        float shadowMapInvSize = GetShadowMapInvSize();
        sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(shadowMapInvSize, 0.0)));
        sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(0.0, shadowMapInvSize)));
        sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(shadowMapInvSize)));
        sampledDistance *= 0.25;
        bool inShadow = sampledDistance / GetLightInvRange() < length(world2light) - u_shadowBias;
        #ifdef COLOR_SPLITS
            const vec4 Red = vec4(1.0, 0.0, 0.0, 1.0);
            const vec4 Green = vec4(0.0, 1.0, 0.0, 1.0);
            const vec4 Blue = vec4(0.0, 0.0, 1.0, 1.0);
            const vec4 RedOrange = vec4(1.0, 69.0/255.0, 0.0, 1.0);
            int split = GetSplit();
            if(split == 0)
                return inShadow ? Red : White;
            else if(split == 1)
                return inShadow ? Green : White;
            else if(split == 2)
                return inShadow ? Blue : White;
            else
                return inShadow ? RedOrange : White;
        #else
            return inShadow ? u_shadowColor : White;
        #endif
    }

#endif
