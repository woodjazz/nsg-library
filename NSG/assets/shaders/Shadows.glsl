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
        //return sampledDistance / u_lightInvRange < length(world2light) - u_shadowBias ? u_shadowColor : vec4(1.0);
        return sampledDistance / u_lightInvRange < length(world2light) ? u_shadowColor : vec4(1.0);
    }

#elif defined(SHADOWMAP)

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

#if 0
    // Based on http://jcgt.org/published/0003/04/08/paper-lowres.pdf
    float CalcAdaptativeDepthBias(int split, vec4 smTexCoord)
    {
        // Shadow map resolution
        float smBufferRes = 1.0/u_shadowMapInvSize[split];

        // Locate corresponding light space shadow map grid center
        vec2  index = floor( vec2( smTexCoord.xy * smBufferRes) );
        float delta = 1.0 / smBufferRes;
        // Normalized coordinate in [0,1]
        vec2  nlsGridCenter = delta*(index + vec2(0.5)); // Normalized eye space grid center --- [0,1]
        // Unnormalized coordinate in [-lightLeft,lightLeft]
        vec2 lsGridCenter = u_lightViewBound*( 2.0*nlsGridCenter - vec2(1.0) );
        
        /** Define light ray **/
        // Light ray direction in light space
        vec3 lsGridLineDir = normalize( vec3(lsGridCenter, -lightNear) ); // Light space grid line direction    
        
        /** Plane ray intersection **/
        // Locate the potential occluder for the shading fragment
        float ls_t_hit = dot(n, lsFragPos.xyz) / dot(n, lsGridLineDir);
        vec3  ls_hit_p = ls_t_hit * lsGridLineDir;
        
        /** Compute Adaptive Epsilon **/
        // Normalized depth value in shadow map
        float SMDepth = texture( shadowTex, smTexCoord.xy );
        // A and B are computed bnased on light near and far planes.
        // They can be retrieved directly from light projection matrix
        float A = lightProj[2][2];
        float B = lightProj[3][2];    
        float adaptiveDepthBias = 0.5*pow(1.0 - A - 2.0*SMDepth, 2)*constantBias / B; 
        
        // Use the intersection point as new look up point
        vec4 lsPotentialoccluder = lightProj * vec4(ls_hit_p, 1.0);
        lsPotentialoccluder      = lsPotentialoccluder/lsPotentialoccluder.w;
        lsPotentialoccluder      = 0.5 * lsPotentialoccluder + vec4(0.5, 0.5, 0.5, 0.0);
       
        float actualDepth = min(lsPotentialoccluder.z, smTexCoord.z);
        float actualBias  = adaptiveDepthBias;
            
        // Constant depth bias VS adaptive depth bias
        actualDepth = adaptiveFlag != 0 ? actualDepth : smTexCoord.z;
        actualBias  = adaptiveFlag != 0 ? actualBias  : zBias;    

    }
#endif

    vec4 CalcShadowFactor()
    {
        const vec4 White = vec4(1.0);

        // Transform from eye-space to shadow map texture coordinates
        vec4 coords = GetShadowClipPos(vec4(v_worldPos, 1.0));
        coords /= coords.w; // Normalize from -w..w to -1..1
        coords  = 0.5 * coords + vec4(0.5, 0.5, 0.5, 0.0); // Normalize from -1..1 to 0..1
        // If region is out of shadowcam frustum then not in shadow
        if(clamp(coords.xyz, 0.0, 1.0) != coords.xyz)
            return White;

        // Take four samples and average them
        float sampledDistance = DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy));

        #if 0
        float shadowMapInvSize = GetShadowMapInvSize();
        sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(shadowMapInvSize, 0.0)));
        sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(0.0, shadowMapInvSize)));
        sampledDistance += DecodeColor2Depth(GetTexture2DFromShadowMap(coords.xy + vec2(shadowMapInvSize)));
        sampledDistance *= 0.25;
        #endif

        bool inShadow = sampledDistance < coords.z;// - u_shadowBias;

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
