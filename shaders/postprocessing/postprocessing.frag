#version 330 core

#import ../fragment/header.frag
#import ../fragment/AgX.frag
#import ../fragment/utils/random.frag

void	main(void)
{
	vec2 uv = vec2(coord.x, 1.0 - coord.y);

	g_seed = int(fract(sin(dot(vec2(coord.xy), vec2(12.9898, 78.233))) * 43758.5453123) * 5929);

	vec3 col = texture(cumulative_render_buffer, uv).rgb;

	// if (rt.mode == MODE_PREVIEW && rt.debug == 0)
	// 	col = to_agx(col.rgb);
	// if (rt.mode == MODE_PREVIEW && rt.debug == 0)
	// 	col = dither(col);

	if (rt.mode == MODE_PREVIEW)
		col = to_agx(col.rgb);
	col = dither(col);

	FragColor = col;
}
