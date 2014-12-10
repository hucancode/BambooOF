uniform sampler2D u_textures[32];
varying vec2 v_uv;
varying int v_texture_id;
void main()
{
	vec4 color = texture2D(u_textures[v_texture_id],v_uv);
    gl_FragColor = color;
}