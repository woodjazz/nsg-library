#if defined(COMPILEFS)

uniform sampler2D u_texture0; /* DIFFUSEMAP */
uniform sampler2D u_texture1; /* NORMALMAP, AOMAP */
uniform sampler2D u_texture2; /* LIGHTMAP, SPECULARMAP */
#if defined(CUBESHADOWMAP)
	uniform samplerCube u_texture3; /* CUBESHADOWMAP */
#elif defined(SHADOWMAP)
uniform sampler2D u_texture3; /* SHADOWMAP0 */
uniform sampler2D u_texture4; /* SHADOWMAP1 */
uniform sampler2D u_texture5; /* SHADOWMAP2 */
uniform sampler2D u_texture6; /* SHADOWMAP3 */
#endif

#endif
