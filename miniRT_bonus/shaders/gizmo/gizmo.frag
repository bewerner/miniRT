#version 330 core

in vec3		gizmo_col;
in float	depth;
in float	circle;
in float	background;
in float	dpi_scale;
in float	mac_os;
in float	debug;
out vec4	FragColor;

vec3		RED;
vec3		GREEN;
vec3		BLUE;


vec4	draw_circle(vec4 col)
{
	if (length(col.rgb) > (1 - depth))
		discard;
	if (length(col.rgb) > (1 - depth) - 0.05)
		col.a = 0.5;
	if (abs(circle) == 1)
		col.rgb = RED;
	else if (abs(circle) == 2)
		col.rgb = GREEN;
	else if (abs(circle) == 3)
		col.rgb = BLUE;
	if (circle < 0 && length(FragColor.rgb) < (1 - depth) - 0.2)
	{
		col.rgb *= 0.5;
		col.a = 1.1 - depth * 8;
	}
	return (col);
}

vec4	draw_background(vec4 col)
{
	float radius  = length(col.rgb);
	float feather = 0.03 / dpi_scale;

	if (radius > 1)
		discard;
	if (radius > 1 - feather)
		col.a = (1 - radius) / feather;
	col.rgb = vec3(1);
	col.a *= 0.21;
	return (col);
}

void	main(void)
{
	if (mac_os == 1)
	{
		RED		= vec3(0.961, 0.301, 0.352);
		GREEN	= vec3(0.618, 0.845, 0.000);
		BLUE	= vec3(0.301, 0.557, 1.000);
	}
	else
	{
		RED		= vec3(1.000, 0.212, 0.325);
		GREEN	= vec3(0.541, 0.859, 0.000);
		BLUE	= vec3(0.173, 0.561, 1.000);
	}
	FragColor = vec4(gizmo_col, 1);
	if (background == 1)
		FragColor = draw_background(FragColor);
	else if (abs(circle) > 0)
		FragColor = draw_circle(FragColor);
	else if (gizmo_col == vec3(1, 0, 0))
		FragColor.rgb = RED;
	else if (gizmo_col == vec3(0, 1, 0))
		FragColor.rgb = GREEN;
	else if (gizmo_col == vec3(0, 0, 1))
		FragColor.rgb = BLUE;
	if (background == 0)
		FragColor.rgb *= 1 - (3.5 * depth);
}
