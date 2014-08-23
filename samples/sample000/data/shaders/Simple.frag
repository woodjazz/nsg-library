
void main()
{
	gl_FragColor = v_color * texture2D(u_texture0, v_texcoord);
}

