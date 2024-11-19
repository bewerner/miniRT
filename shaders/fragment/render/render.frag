#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 out_camera_ray_dir;			// rgb is ray direction. alpha is object offset (-1 if no hit)
layout(location = 2) out vec4 out_previous_ray_dir;			// rgb is ray direction. alpha is object offset (-1 if no hit)
layout(location = 3) out vec4 out_previous_ray_origin;		// alpha is ???
layout(location = 4) out vec4 out_render;					// alpha is ???
layout(location = 5) out vec4 out_specular;					// alpha is ???
layout(location = 6) out vec4 out_camera_ray_origin;		// alpha is ???
layout(location = 7) out vec4 out_diffuse;					// alpha is ???

#import header.frag
#import checker_texture.frag
#import noise_texture.frag
#import utils/environment.frag
#import trace_ray.frag

#import objects/object_utils.frag
#import objects/point_light.frag
#import objects/sphere.frag
#import objects/plane.frag
#import objects/cylinder.frag
#import objects/hyperboloid.frag

#import utils/sample.frag
#import utils/normal_map.frag
#import utils/ray_utils.frag
#import utils/hitpoint_utils.frag
#import utils/vec3_rotations.frag
#import utils/random.frag

t_ray	get_camera_ray(void)
{
	t_ray	ray;

	vec2 screen = (coord * 2 - 1);
	vec2 pixel_distance = vec2(2.0) / vec2(rt.width, rt.height);
	if (rt.sample_count > 1)
			screen += pixel_distance * sample_uniform_disc(1.5); // Film Pixel Filter Width = 1.5
	screen /= vec2(1, rt.aspect_ratio);

	ray.origin = rt.camera.origin;
	if (rt.camera.f_stop > 0)
	{
		float aperture = (rt.camera.focal_length * 0.018) / rt.camera.f_stop; // screen size (2000mm) * 0.018 = sensor size (36mm)
		vec2 defocus = sample_uniform_disc(aperture);
		ray.origin += defocus.x * rt.camera.right + defocus.y * rt.camera.up;
	}

	vec3 target = (rt.camera.origin + rt.camera.direction * rt.camera.focal_length) + (screen.x * rt.camera.right) + (screen.y * rt.camera.up);
	target = normalize(target - rt.camera.origin);
	target *= rt.camera.focus_distance;
	target += rt.camera.origin;

	ray.dir = target - ray.origin;

	out_camera_ray_dir.rgb = ray.dir;
	out_camera_ray_origin.rgb = ray.origin;

	return (ray);
}

void	main(void)
{
	vec2 uv = vec2(coord.x, 1.0 - coord.y);

	FragColor               = texture(buffer, vec3(uv, 0.0)).rgba;
	out_camera_ray_dir		= texture(buffer, vec3(uv, 1.0)).rgba;
	out_previous_ray_dir	= texture(buffer, vec3(uv, 2.0)).rgba;
	out_previous_ray_origin	= texture(buffer, vec3(uv, 3.0)).rgba;
	out_render				= texture(buffer, vec3(uv, 4.0)).rgba;
	out_specular			= texture(buffer, vec3(uv, 5.0)).rgba;
	out_camera_ray_origin	= texture(buffer, vec3(uv, 6.0)).rgba;
	out_diffuse				= texture(buffer, vec3(uv, 7.0)).rgba;
	FragColor.a = 1.0;

	bool previous_hit = bool(out_previous_ray_dir.a + 1);

	init_seed();

	out_render.rgb = trace_camera_ray(get_camera_ray());

	out_previous_ray_dir = out_camera_ray_dir;
	out_previous_ray_origin = out_camera_ray_origin;
}
