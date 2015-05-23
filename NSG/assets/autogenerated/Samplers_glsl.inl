#pragma once
namespace NSG
{
static const char* SAMPLERS_GLSL = \
"#if defined(COMPILEFS)\n"\
"uniform sampler2D u_texture0; /* DIFFUSEMAP */\n"\
"uniform sampler2D u_texture1; /* NORMALMAP */\n"\
"uniform sampler2D u_texture2; /* LIGHTMAP */\n"\
"uniform sampler2D u_texture3; /* SPECULARMAP */\n"\
"uniform sampler2D u_texture4; /* AOMAP */\n"\
"#ifdef CUBESHADOWMAP\n"\
"	uniform samplerCube u_texture5; /* CUBESHADOWMAP */\n"\
"#else\n"\
"	uniform sampler2D u_texture5; /* SHADOWMAP */\n"\
"#endif\n"\
"#endif\n"\
;
}
