#version 330 core

#import ../fragment/header.frag

#import ../fragment/get_diffuse_color.frag

#import ../fragment/checker_texture.frag
#import ../fragment/get_illumination_color.frag
#import get_solid_diffuse_color.frag
#import get_solid_specular_color.frag
#import light_gizmo.frag
#import ../fragment/objects/object_utils.frag
#import ../fragment/objects/sphere.frag
#import ../fragment/objects/plane.frag
#import ../fragment/objects/cylinder.frag
#import ../fragment/objects/hyperboloid.frag
#import ../fragment/objects/point_light.frag
#import trace_ray_solid.frag

#import ../fragment/utils/normal_map.frag
#import ../fragment/utils/ray_utils.frag
#import ../fragment/utils/hitpoint_utils.frag
#import ../fragment/utils/vec3_rotations.frag
#import ../fragment/utils/random.frag

vec3	get_solid_specular_color(t_point_light light, float roughness, t_hitpoint hitpoint);
vec3	get_solid_color(t_hitpoint hitpoint);
vec3	trace_ray_solid(t_ray ray, vec2 uv);

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


	if (is_ligth_gizmo(camera_ray) == true)
	{
		FragColor = VEC3_BLACK;
		return ;
	}

	vec3 col = trace_ray_solid(camera_ray, uv);

	FragColor = col;
}
