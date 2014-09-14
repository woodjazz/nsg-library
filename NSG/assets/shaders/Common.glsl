//Remember to rebuild with CMake if this file changes
#ifdef GLES2
	#ifdef GL_FRAGMENT_PRECISION_HIGH
		precision highp float;
		precision highp int;
	#else
		precision mediump float;
		precision mediump int;
	#endif
#else
	#define lowp
	#define mediump
	#define highp
#endif   

varying vec4 v_color;
varying vec3 v_normal;
varying vec3 v_tangent;
varying vec3 v_bitangent;
varying vec2 v_texcoord; 
varying vec3 v_vertexToEye;
varying vec3 v_lightDirection[MAX_POINT_LIGHTS];

#if defined(COMPILEFS)
	
	uniform sampler2D u_texture0;
	uniform sampler2D u_texture1;
	
#endif