uniform mat4 u_projection_matrix;
uniform mat4 u_modelview_matrix;
uniform mat4 u_transform_matrix;
uniform mat4 u_cam_inverse_matrix;
attribute vec3 a_position;
uniform int u_texture_count;
attribute vec4 a_uv[15];
varying vec4 v_uv[15];
void main()
{
	for(int i=0;i<u_texture_count;i++)
	{
		v_uv[i] = a_uv[i];
	}
	gl_Position = u_transform_matrix*vec4(a_position,1.0);
}