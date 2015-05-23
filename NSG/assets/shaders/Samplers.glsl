#if defined(COMPILEFS)

uniform sampler2D u_texture0; /* DIFFUSEMAP */
uniform sampler2D u_texture1; /* NORMALMAP */
uniform sampler2D u_texture2; /* LIGHTMAP */
uniform sampler2D u_texture3; /* SPECULARMAP */
uniform sampler2D u_texture4; /* AOMAP */
#ifdef CUBESHADOWMAP
	uniform samplerCube u_texture5; /* CUBESHADOWMAP */
#else
	uniform sampler2D u_texture5; /* SHADOWMAP */
#endif

#endif
