uniform mat4 u_projection_matrix;
uniform mat4 u_modelview_matrix;
uniform mat4 u_transform_matrix;
uniform mat4 u_cam_inverse_matrix;
attribute vec3 a_position;
attribute vec2 a_uv;
attribute int a_texture_id;
varying vec2 v_uv;
varying int v_texture_id;
void main()
{
	v_uv = a_uv;
	v_texture_id = a_texture_id;
	gl_Position = u_transform_matrix*vec4(a_position,1.0);
}