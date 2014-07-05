uniform int u_texture_count;
uniform sampler2D u_texture[15];
varying vec4 v_uv[15];
void main()
{
	vec4 final_color = vec4(0.0,0.0,0.0,0.0);
	for(int i=0;i<u_texture_count;i++)
	{
		vec2 uv = v_uv[i].xy;
		vec2 cuv = v_uv[i].zw;
		if(cuv.x < 0.0 || cuv.x > 1.0 || cuv.y < 0.0 || cuv.y > 1.0) continue;
		vec4 color = texture2D(u_texture[i],uv);
		if(color.a == 0.0) continue;
		final_color += color;
		if(final_color.a >= 1.0) break;
	}
	if(final_color.a == 0.0) discard;
    gl_FragColor = final_color;
}