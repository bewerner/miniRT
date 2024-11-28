#version 330 core

#import ../render/header.frag

#import ../render/checker_texture.frag
#import get_normal_color.frag
#import ../render/objects/object_utils.frag
#import ../render/objects/sphere.frag
#import ../render/objects/plane.frag
#import ../render/objects/cylinder.frag
#import ../render/objects/hyperboloid.frag
#import trace_ray_normal.frag

#import ../render/utils/normal_map.frag
#import ../render/utils/hitpoint_utils.frag
#import ../render/utils/vec3_rotations.frag
#import ../render/utils/random.frag

vec3	get_normal_color(t_hitpoint hitpoint);
vec3	trace_ray_normal(t_ray ray);

out vec4 FragColor;

void main()
{
	vec2 uv = coord.xy;
	uv.x = uv.x * 2 - 1.0;
	uv.y = (uv.y * 2 - 1.0) / rt.aspect_ratio;
	g_seed = int(fract(sin(dot(vec2(coord.xy), vec2(12.9898, 78.233))) * 43758.5453123) * 5929);

	t_ray camera_ray;
	camera_ray.origin = rt.camera.origin;
	vec3 camera_up = cross(rt.camera.direction, rt.camera.right);
	camera_ray.dir = uv.y * camera_up + uv.x * rt.camera.right + rt.camera.focal_length * rt.camera.direction;

	vec3 col = trace_ray_normal(camera_ray);

	FragColor.rgb = col;
}
