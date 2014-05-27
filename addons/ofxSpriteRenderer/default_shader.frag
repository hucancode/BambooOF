precision mediump float;
uniform int u_texture_count;
uniform sampler2D u_texture[32];
varying vec2 v_uv[32];
varying vec2 v_cuv[32];
void main()
{
	vec4 final_color = vec4(0.0,0.0,0.0,0.0);
	for(int i=0;i<u_texture_count;i++)
	{
		vec2 cuv = v_cuv[i];
		if(cuv.x < 0.0) break;
        if(cuv.x > 1.0) break;
		if(cuv.y < 0.0) break;
        if(cuv.y > 1.0) break;
		sampler2D texture = u_texture[i];
		vec2 uv = v_uv[i];
		vec4 color = texture2D(texture,uv);
		// blend or do something with final color
		if(final_color.a >= 1.0) break;
	}
    gl_FragColor = final_color;
}