#version 330 core

#import ../fragment/header.frag
#import ../fragment/AgX.frag
#import ../fragment/utils/random.frag

void	main(void)
{
	vec2 uv = vec2(coord.x, 1.0 - coord.y);

	// Dump unused samplerBuffer 
	float dump_value_objects = texelFetch(objects, 0).r;
	float dump_value_lights = texelFetch(lights, 0).r;

	g_seed = int(fract(sin(dot(vec2(coord.xy), vec2(12.9898, 78.233))) * 43758.5453123) * 5929 * (rt.sample_count + 1)) + rt.sample_count * 9823;

	vec3 col = texture(raw_render_image, uv).rgb;
	// if (rt.sample_count < rt.max_samples)
	col = to_agx(col.rgb);
	col = dither(col);
	FragColor = col;
}
