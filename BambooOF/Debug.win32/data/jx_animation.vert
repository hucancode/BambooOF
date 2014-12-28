uniform mat4 u_transform_matrix;
attribute vec3 a_position;
attribute vec2 a_uv;
varying vec2 v_uv;
void main()
{
	v_uv = a_uv;
	gl_Position = u_transform_matrix*vec4(a_position,1.0);
}