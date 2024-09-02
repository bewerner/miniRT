#version 330 core

#import header.frag
#import get_diffuse_color.frag
#import get_reflection_color.frag
#import get_specular_color.frag
#import point_light.frag
#import sphere.frag
#import plane.frag
#import cylinder.frag
#import trace_ray.frag
#import vec3_rotations.frag
#import random.frag

vec4 to_agx(vec4 col)
{
	col = round(col * 10000);
	col = clamp(col, 0.0, 165000.0);
	col.r = texelFetch(agx_lut, int(col.r)).r;
	col.g = texelFetch(agx_lut, int(col.g)).r;
	col.b = texelFetch(agx_lut, int(col.b)).r;
	col = pow(col, vec4(1 / 2.2));
	col -= pow(1 - col, vec4(5)) * 0.04;
	col.a = 1.0;
	return (col);
}

void	main(void)
{
	vec2 uv = coord.xy;
	uv.x = uv.x * 2 - 1.0;
	uv.y = (uv.y * 2 - 1.0) / rt.aspect_ratio;

	g_seed = int(fract(sin(dot(vec2(coord.xy), vec2(12.9898, 78.233))) * 43758.5453123) * 5929);
	// g_metallic = rt.debug/10.0;

	t_ray camera_ray;
	camera_ray.origin = rt.camera.origin;
	vec3 camera_up = cross(rt.camera.direction, rt.camera.right);
	camera_ray.dir = uv.y * camera_up + uv.x * rt.camera.right + rt.camera.focal_length * rt.camera.direction;

	vec4 col = trace_ray(camera_ray);
	col = to_agx(col);
	col = dither(col);

	// int x = int(rt.debug) * 1;
	// col.r = texelFetch(agx_lut, x).r;
	// // for (int i = 0; i < rt.debug; i++)
	// // 	col.r = rand();
	// col.g = col.r;
	// col.b = col.r;
	FragColor = col;
}
