#version 330 core

#import ../fragment/header.frag
#import ../fragment/AgX.frag
#import ../fragment/utils/random.frag

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
	vec2 uv = vec2(coord.x, 1.0 - coord.y);

	g_seed = int(fract(sin(dot(vec2(coord.xy), vec2(12.9898, 78.233))) * 43758.5453123) * 5929);

	vec3 col = texture(raw_render_image, uv).rgb;
	if (rt.mode == MODE_PREVIEW)
		col = to_agx(col.rgb);
	else if (rt.debug == 1)
		col = adjust_for_p3_display(col);
	col = dither(col);
	FragColor = col;
}
