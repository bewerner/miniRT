#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

uniform float	u_pitch;
uniform float	u_yaw;
uniform float	u_aspect_ratio;
uniform float	u_scale;
uniform float	u_dpi_scale;
uniform int		u_selected;
uniform int		u_mac_os;
uniform float	u_debug;

out vec3	gizmo_col;
out float	depth;
out float	circle;
out float	background;
out float	dpi_scale;
out float	mac_os;
out	float	debug;

void main()
{
	depth = 0;
	circle = 0;
	background = 0;
	debug = u_debug;
	dpi_scale = u_dpi_scale;
	mac_os = u_mac_os;

	if (aCol.b == 0.5)
		background = 1;

	mat3 mat_rot_x = mat3
	(
		1.0,	0.0,			0.0,
		0.0,	cos(u_pitch),	-sin(u_pitch),
		0.0,	sin(u_pitch),	cos(u_pitch)
	);

	mat3 mat_rot_x2 = mat3
	(
		1.0,	0.0,			0.0,
		0.0,	cos(-u_pitch - 3.14159265359 / 2),	-sin(-u_pitch - 3.14159265359 / 2),
		0.0,	sin(-u_pitch - 3.14159265359 / 2),	cos(-u_pitch - 3.14159265359 / 2)
	);

	mat3 mat_rot_y = mat3
	(
		cos(-u_pitch),	0.0,	sin(-u_pitch),
		0.0,			1.0,	0.0,
		-sin(-u_pitch),	0.0,	cos(-u_pitch)
	);

	mat3 mat_rot_z = mat3
	(
		cos(u_yaw),		-sin(u_yaw),	0.0,
		sin(u_yaw),		cos(u_yaw),		0.0,
		0.0,			0.0,			1.0
	);	

	mat3 mat_rot_z2 = mat3
	(
		cos(-u_yaw),	-sin(-u_yaw),	0.0,
		sin(-u_yaw),	cos(-u_yaw),	0.0,
		0.0,			0.0,			1.0
	);	

	vec3 pos = aPos;
	if (aCol.b >= 2)
		circle = aCol.b - 1;
	else if (aCol.b <= -2)
		circle = aCol.b + 1;
	if (abs(circle) >= 1)
	{
		pos *= mat_rot_x2;
		pos *= mat_rot_z2;
	}
	if (circle == 1)
		pos.x += 100;
	else if (circle == 2)
		pos.y += 100;
	else if (circle == 3)
		pos.z -= 100;
	else if (circle == -1)
		pos.x -= 100;
	else if (circle == -2)
		pos.y -= 100;
	else if (circle == -3)
		pos.z += 100;

	// rotate
	if (aCol.b != 1 && background == 0)
		pos *= mat_rot_z;
	if (background == 0)
		pos *= mat_rot_x;

	// set depth
	depth = (pos.z / (100) + 1) / 2;
	depth *= 0.1;

	pos *= u_scale;				// apply scale
	pos.y *= u_aspect_ratio;	// apply aspect ratio
	pos.xy += 1;				// move to upper right corner of window
	pos.x -= 161 * u_scale;		// move inwards
	pos.y -= 161 * u_scale * u_aspect_ratio;

	gl_Position = vec4(pos, 1.0);
	gizmo_col = aCol;
	if (abs(circle) >= 1 || background == 1)
		gizmo_col.b = 0;
	if (background == 1 && u_selected == 0)
		gl_Position = vec4(99);
}
