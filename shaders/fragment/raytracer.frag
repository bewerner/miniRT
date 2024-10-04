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

#import utils/normal_map.frag
#import utils/ray_utils.frag
#import utils/hitpoint_utils.frag
#import utils/vec3_rotations.frag
#import utils/random.frag

void	main(void)
{
	vec2 uv = coord;
	uv.x = uv.x * 2 - 1.0;
	uv.y = (uv.y * 2 - 1.0) / rt.aspect_ratio;
	vec2 tmp = vec2(coord.x, 1.0 - coord.y);

	g_seed = int(fract(sin(dot(vec2(coord.xy), vec2(12.9898, 78.233))) * 43758.5453123) * 5929 * (rt.sample_count + 1)) + rt.sample_count * 9823;
	g_seed += int(rand() * 943 * rt.sample_count);

	t_ray camera_ray;
	camera_ray.origin = rt.camera.origin;
	vec3 camera_up = cross(rt.camera.direction, rt.camera.right);
	camera_ray.dir = uv.y * camera_up + uv.x * rt.camera.right + rt.camera.focal_length * rt.camera.direction;
	camera_ray.dir.x = camera_ray.dir.x - (2.0 / rt.width  / 2.0) + (2.0 / rt.width  * rand() * 1);
	camera_ray.dir.y = camera_ray.dir.y - (2.0 / rt.height / 2.0) + (2.0 / rt.height * rand() * 1);

	vec3 col;
	col = trace_ray(camera_ray);

	// if (rt.debug == 0)
	// 	col = vec3(rand());
	FragColor = mix(texture(raw_render_image, tmp).rgb, col, 1.0 / rt.sample_count);
	uv.x = coord.x;
	uv.y = 1 - coord.y;

	// if (rt.debug == 1)
	// 	FragColor = texture(environment_map, uv).rgb;
	// if (rt.debug == 2)
	// 	FragColor = vec3(texture(environment_map, uv).a);
	// if (rt.debug == 3)
	// 	FragColor = vec3(get_pdf(ivec2(uv.x * 2048, uv.y * 1024), 2048, 1024) * 1000000);

	// if (rt.debug == 4)
	// 	FragColor = vec3(texelFetch(environment_map, ivec2(0,0), 0).a);
	// if (rt.debug == 5)
	// 	FragColor = vec3(texture(environment_map, vec2(0,0)).a);

	// 	// FragColor = vec3(texture(environment_map, vec2(1.0/2048,1.0/1024)).a);
	// if (rt.debug == -2)
	// 	FragColor = vec3(texture(environment_map, vec2(0.8,0.8)).a);
}
