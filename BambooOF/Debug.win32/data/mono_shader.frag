uniform sampler2D u_texture;
varying vec4 v_uv;
void main()
{
	if(v_uv.z < 0 || v_uv.z > 1 || v_uv.w < 0 || v_uv.w > 1) discard;
	vec4 final_color = texture2D(u_texture, v_uv.xy);
	if(final_color.a == 0) discard;
    gl_FragColor = final_color;
}