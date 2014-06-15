precision mediump float;
uniform sampler2D u_texture;
varying vec2 v_uv;
varying vec2 v_cuv;
void main()
{
	if(v_cuv.x < 0.0) discard;
    if(v_cuv.x > 1.0) discard;
	if(v_cuv.y < 0.0) discard;
    if(v_cuv.y > 1.0) discard;
	vec4 final_color = texture2D(u_texture, v_uv);
    gl_FragColor = final_color;
}