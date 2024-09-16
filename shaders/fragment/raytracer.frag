#version 330 core

#import header.frag
#import AgX.frag
#import checker_texture.frag
#import get_illumination_color.frag
#import get_sky_color.frag
#import get_diffuse_color.frag
#import get_reflection_color.frag
#import get_specular_color.frag
#import trace_ray.frag

#import objects/object_utils.frag
#import objects/point_light.frag
#import objects/sphere.frag
#import objects/plane.frag
#import objects/cylinder.frag
#import objects/hyperboloid.frag

#import utils/ray_utils.frag
#import utils/hitpoint_utils.frag
#import utils/vec3_rotations.frag
#import utils/random.frag

void	main(void)
{
	vec2 uv = coord;
	uv.x = uv.x * 2 - 1.0;
	uv.y = (uv.y * 2 - 1.0) / rt.aspect_ratio;

	g_seed = int(fract(sin(dot(vec2(coord.xy) * rt.frame_count, vec2(12.9898, 78.233))) * 43758.5453123) * 5929);

	// Add pixel randomization to the rays for color accumulation (bouncelight, antialiasing, transmission, emission)
	uv.x += (1 / rt.screen_size.x) * rand();
	uv.y += (1 / rt.screen_size.y) * rand();

	t_ray camera_ray;
	camera_ray.origin = rt.camera.origin;
	vec3 camera_up = cross(rt.camera.direction, rt.camera.right);
	camera_ray.dir = uv.y * camera_up + uv.x * rt.camera.right + rt.camera.focal_length * rt.camera.direction;

	vec3 col = trace_ray(camera_ray);
	col = to_agx(col.rgb);
	col = dither(col);

	FragColor = col;
}
