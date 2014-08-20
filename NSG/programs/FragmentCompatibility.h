#ifdef GL_ES_VERSION_2_0
"#version 100\n"
"#define GLES2\n"
"#ifdef GL_FRAGMENT_PRECISION_HIGH\n"
	"precision highp float;\n"
"#else\n"
	"precision mediump float;\n"
"#endif\n"
#else
"#version 120\n"
"#define lowp   \n"
"#define mediump\n"
"#define highp  \n"
#endif
"varying vec4 v_color;\n"
"varying vec3 v_normal;\n"
"varying vec2 v_texcoord;\n"
"uniform sampler2D u_texture0;\n"
"uniform sampler2D u_texture1;\n"
