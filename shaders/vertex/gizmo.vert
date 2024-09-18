#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;

uniform float u_pitch;
uniform float u_yaw;

out vec3	gizmo_col;

void main()
{
    // --- Yaw rotation matrix (around Y-axis) ---
    mat4 yawMatrix = mat4(
        cos(u_yaw),  0.0, sin(u_yaw), 0.0,
        0.0,       1.0,       0.0, 0.0,
       -sin(u_yaw),  0.0, cos(u_yaw), 0.0,
        0.0,       0.0,       0.0, 1.0
    );

    // --- Pitch rotation matrix (around X-axis) ---
    mat4 pitchMatrix = mat4(
        1.0,      0.0,       0.0, 0.0,
        0.0, cos(u_pitch), -sin(u_pitch), 0.0,
        0.0, sin(u_pitch),  cos(u_pitch), 0.0,
        0.0,      0.0,       0.0, 1.0
    );

    // Apply both rotations to the vertex position
	vec3 pos = aPos * 0.1;
    gl_Position = yawMatrix * pitchMatrix * vec4(pos, 1.0);

	// gl_Position = vec4(aPos, 1.0);
	// gl_Position.x += u_pitch;
	// gl_Position.y += u_yaw;
	gizmo_col = aCol;
}
