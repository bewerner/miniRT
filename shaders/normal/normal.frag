#version 330 core

#import ../fragment/header.frag
#import get_normal_color.frag
#import ../fragment/sphere.frag
#import ../fragment/plane.frag
#import ../fragment/cylinder.frag
#import trace_ray_normal.frag
#import ../fragment/vec3_rotations.frag
#import ../fragment/random.frag

vec4	get_normal_color(t_hitpoint hitpoint);
vec4	trace_ray_normal(t_ray ray);
 
void main()
{
	vec2 uv = coord.xy;
	uv.x = uv.x * 2 - 1.0;
	uv.y = (uv.y * 2 - 1.0) / rt.aspect_ratio;

	t_ray camera_ray;
	camera_ray.origin = rt.camera.origin;
	vec3 camera_up = cross(rt.camera.direction, rt.camera.right);
	camera_ray.dir = uv.y * camera_up + uv.x * rt.camera.right + rt.camera.focal_length * rt.camera.direction;

	vec4 col = trace_ray_normal(camera_ray);

	FragColor = col;
}
