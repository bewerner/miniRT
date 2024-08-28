#version 330 core

#import ../fragment/header.frag

void main()
{
	vec2 uv = coord.xy;
	uv.x = uv.x * 2 - 1.0;
	uv.y = (uv.y * 2 - 1.0) / rt.aspect_ratio;

	FragColor = vec4(coord.xy, 0, 1);
}
