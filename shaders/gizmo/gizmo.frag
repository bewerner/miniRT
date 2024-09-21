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

vec3	adjust_for_p3_display(vec3 col)
{
	mat3 m = mat3
	(
		0.822593,	0.177534,	0.000000,
		0.033200,	0.966783,	0.000000,
		0.017085,	0.072396,	0.910301
	);

	return (col * m);
}

void	main(void)
{
	if (mac_os == 1)
	{
		RED		= vec3(1, 0.212, 0.325);
		GREEN	= vec3(0.541, 0.859, 0);
		BLUE	= vec3(0.173, 0.561, 1);
	}
	else
	{
		RED		= vec3(0.961, 0.216, 0.216);
		GREEN	= vec3(0.435, 0.643, 0.102);
		BLUE	= vec3(0.184, 0.518, 0.890);
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
	if (debug == 1)
		FragColor.rgb = adjust_for_p3_display(FragColor.rgb);
}
