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
varying vec2 v_texcoord0; 
varying vec2 v_texcoord1;
varying vec3 v_vertexToEye;
varying vec3 v_lightDirection[NUM_POINT_LIGHTS_ARRAY];
varying vec3 v_light2Pixel[NUM_SPOT_LIGHTS_ARRAY];

#if defined(COMPILEFS)
	
	uniform sampler2D u_texture0;
	uniform sampler2D u_texture1;
	uniform sampler2D u_texture2;
	
#endif