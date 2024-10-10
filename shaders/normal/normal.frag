#version 330 core


#import ../fragment/header.frag

#import ../fragment/checker_texture.frag
#import get_normal_color.frag
#import ../fragment/objects/object_utils.frag
#import ../fragment/objects/sphere.frag
#import ../fragment/objects/plane.frag
#import ../fragment/objects/cylinder.frag
#import ../fragment/objects/hyperboloid.frag
#import trace_ray_normal.frag

#import ../fragment/utils/normal_map.frag
#import ../fragment/utils/hitpoint_utils.frag
#import ../fragment/utils/vec3_rotations.frag
#import ../fragment/utils/random.frag

vec3	get_normal_color(t_hitpoint hitpoint);
vec3	trace_ray_normal(t_ray ray);
 
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

	FragColor = col;
}
