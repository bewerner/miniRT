#version 330 core

#import ../fragment/header.frag
#import ../fragment/AgX.frag
#import ../fragment/utils/random.frag

out vec3 FragColor;

void	main(void)
{
	vec2 uv = vec2(coord.x, 1.0 - coord.y);

	g_seed = int(fract(sin(dot(vec2(coord.xy), vec2(12.9898, 78.233))) * 43758.5453123) * 5929);

	vec3 col = texture(buffer, vec3(uv, 0.0)).rgb;

	if (rt.mode == MODE_PREVIEW)
		col = to_agx(col.rgb);
	col = dither(col);

	FragColor = col;
}