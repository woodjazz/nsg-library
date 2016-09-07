#pragma once
namespace NSG
{
static const char* SAMPLERS_GLSL = \
"#if defined(COMPILEFS)\n"\
"uniform sampler2D u_texture0; /* DIFFUSEMAP */\n"\
"uniform sampler2D u_texture1; /* NORMALMAP, AOMAP */\n"\
"uniform sampler2D u_texture2; /* LIGHTMAP, SPECULARMAP */\n"\
"#if defined(CUBESHADOWMAP)\n"\
"	uniform samplerCube u_texture3; /* CUBESHADOWMAP */\n"\
"#elif defined(SHADOWMAP)\n"\
"uniform sampler2D u_texture3; /* SHADOWMAP0 */\n"\
"uniform sampler2D u_texture4; /* SHADOWMAP1 */\n"\
"uniform sampler2D u_texture5; /* SHADOWMAP2 */\n"\
"uniform sampler2D u_texture6; /* SHADOWMAP3 */\n"\
"#endif\n"\
"#endif\n"\
;
}
