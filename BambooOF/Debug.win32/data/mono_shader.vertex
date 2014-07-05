uniform mat4 u_projection_matrix;
uniform mat4 u_modelview_matrix;
uniform mat4 u_transform_matrix;
uniform mat4 u_cam_inverse_matrix;
attribute vec3 a_position;
attribute vec4 a_uv;
varying vec4 v_uv;
void main()
{
	v_uv = a_uv;
	//vec4 inverse_rotation = u_cam_inverse_matrix*vec4(a_position,1.0);
	gl_Position = u_transform_matrix*vec4(a_position,1.0);
}