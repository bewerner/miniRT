#version 330 core

#import header.frag
#import get_diffuse_color.frag
#import point_light.frag
#import sphere.frag
#import plane.frag
#import cylinder.frag
#import trace_ray.frag
#import vec3_rotations.frag
#import random.frag

void main()
{
	vec2 uv = coord.xy;
	uv.x = uv.x * 2 - 1.0;
	uv.y = (uv.y * 2 - 1.0) / rt.aspect_ratio;

	g_seed = int(sin(dot(coord.xy, vec2(12.9898, 78.233))) * 43758.5453);

	t_ray camera_ray;
	camera_ray.origin = rt.camera.origin;
	vec3 camera_up = cross(rt.camera.direction, rt.camera.right);
	camera_ray.dir = uv.y * camera_up + uv.x * rt.camera.right + rt.camera.focal_length * rt.camera.direction;

	vec4 col = trace_ray(camera_ray);
	col = dither(col);

	// col.r = rand();
	// col.g = col.r;
	// col.b = col.r;
	FragColor = col;
}
