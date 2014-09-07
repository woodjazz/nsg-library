#ifdef 	
	attribute vec4 a_position;
	attribute vec2 a_texcoord;
	attribute vec3 a_normal;
	attribute vec4 a_color;
	varying vec4 v_color;
	varying vec3 v_normal;
	varying vec2 v_texcoord;
#else
	varying vec4 v_color;
	varying vec3 v_normal;
	varying vec2 v_texcoord;
#endif

vec3 GetWorldPos(mat4 modelMatrix)
{
    #if defined(INSTANCED)
    	return (a_position * modelMatrix).xyz;
    #else
		//instancing model matrix is a transpose, so the matrix multiply order must be swapped
       return (modelMatrix * a_position).xyz;
   #endif
}