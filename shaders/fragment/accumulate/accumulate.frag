#version 330 core

#import ../render/header.frag

out vec4 FragColor;

void	main(void)
{
	vec2 uv = vec2(coord.x, 1.0 - coord.y);

	vec3 col_accumulated = texture(buffer, vec3(uv, 0.0)).rgb;
	vec3 col_new = texture(buffer, vec3(uv, 4.0)).rgb;

	FragColor.rgb = mix(col_accumulated, col_new, 1.0 / rt.sample_count);
	FragColor.a = 1.0;
}
