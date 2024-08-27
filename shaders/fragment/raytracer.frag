#version 330 core

#import shaders/fragment/header.frag
#import shaders/fragment/vec3_rotations.frag

void main()
{
	vec2 uv = coord.xy;
	uv.x = uv.x * 2 - 1.0;
	uv.y = (uv.y * 2 - 1.0) / rt.aspect_ratio;

	t_ray camera_ray;
	camera_ray.origin = rt.camera.origin;
	vec3 camera_up = cross(rt.camera.direction, rt.camera.right);
	camera_ray.dir = uv.y * camera_up + uv.x * rt.camera.right + rt.camera.focal_length * rt.camera.direction;

	vec4 col = trace_ray(camera_ray);

	FragColor = col;
}
