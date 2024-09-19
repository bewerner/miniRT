#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

uniform float u_pitch;
uniform float u_yaw;
uniform float u_aspect_ratio;

out vec3	gizmo_col;

void main()
{	mat3 mat_rot_x = mat3
	(
		1.0,	0.0,			0.0,
		0.0,	cos(u_pitch),	-sin(u_pitch),
		0.0,	sin(u_pitch),	cos(u_pitch)
	);

	// mat3 mat_rot_y = mat3
	// (
	// 	cos(u_yaw),	0.0,	sin(u_yaw),
	// 	0.0,			1.0,	0.0,
	// 	-sin(u_yaw),	0.0,	cos(u_yaw)
	// );

	mat3 mat_rot_z = mat3
	(
		cos(u_yaw),		-sin(u_yaw),	0.0,
		sin(u_yaw),		cos(u_yaw),		0.0,
		0.0,			0.0,			1.0
	);	

	vec3 pos = aPos * 0.001;
	if (aCol.b != 1)
		pos *= mat_rot_z;
	pos *= mat_rot_x;
	gl_Position = vec4(pos, 1.0);
	gl_Position.y *= u_aspect_ratio;
	gizmo_col = aCol;
}
