#version 330 core

in vec3		gizmo_col;
in float	depth;
in float	circle;
in float	debug;
out vec4	FragColor;

vec4	draw_circle(vec4 col)
{
	if (length(col.rgb) >= (1 - depth) - 0.05)
		col.a = 0.5;
	if (length(col.rgb) >= (1 - depth))
		discard;
	if (abs(circle) == 1)
		col.rgb = vec3(1, 0.212, 0.325);
	else if (abs(circle) == 2)
		col.rgb = vec3(0.541, 0.859, 0);
	else if (abs(circle) == 3)
		col.rgb = vec3(0.173, 0.561, 1);
	if (circle < 0 && length(FragColor.rgb) < (1 - depth) - 0.15)
	{
		col.rgb *= 0.5;
		col.a = 1.1 - depth * 8;
	}
	return (col);
}

void	main(void)
{
	FragColor = vec4(gizmo_col, 1);
	if (abs(circle) > 0)
		FragColor = draw_circle(FragColor);
	FragColor.rgb *= 1 - (3.5 * depth);
}
