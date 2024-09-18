#version 330 core

in vec3 gizmo_col;
out vec3 FragColor;

void	main(void)
{
	FragColor = gizmo_col;
}
