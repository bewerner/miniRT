#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 out_hitpoint_pos;				// alpha is hitpoint.hit
layout(location = 2) out vec4 out_hitpoint_normal;			// alpha is hitpoint.material_idx
layout(location = 3) out vec4 out_hitpoint_render;			// alpha is glossy_hitpoint metallic
layout(location = 4) out vec4 out_hitpoint_color;			// this doubles as glossy_specular_accumulation
layout(location = 5) out vec4 out_glossy_hitpoint_pos;		// alpha is hitpoint.hit
layout(location = 6) out vec4 out_glossy_hitpoint_normal;	// alpha is hitpoint.material_idx
layout(location = 7) out vec4 out_glossy_hitpoint_ray;		// alpha is glossy_hitpoint roughness

#import header.frag
#import checker_texture.frag
#import noise_texture.frag
#import get_sky_color.frag
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

t_ray	get_camera_ray(void)
{
	t_ray	ray;

	vec2 screen = (coord * 2 - 1);
	vec2 pixel_distance = vec2(2.0) / vec2(rt.width, rt.height);
	if (rt.sample_count > 1)
			screen += pixel_distance * random_point_in_circle(1.5); // Film Pixel Filter Width = 1.5
	screen /= vec2(1, rt.aspect_ratio);

	ray.origin = rt.camera.origin;
	if (rt.camera.f_stop > 0)
	{
		float aperture = (rt.camera.focal_length * 0.018) / rt.camera.f_stop; // screen size (2000mm) * 0.018 = sensor size (36mm)
		vec2 defocus = random_point_in_circle(aperture);
		ray.origin += defocus.x * rt.camera.right + defocus.y * rt.camera.up;
	}

	vec3 target = (rt.camera.origin + rt.camera.direction * rt.camera.focal_length) + (screen.x * rt.camera.right) + (screen.y * rt.camera.up);
	target = normalize(target - rt.camera.origin);
	target *= rt.camera.focus_distance;
	target += rt.camera.origin;

	ray.dir = target - ray.origin;

	return (ray);
}

void	main(void)
{
	vec2 uv = vec2(coord.x, 1.0 - coord.y);

	out_hitpoint_pos			= texture(buffer, vec3(uv, 1.0)).rgba;
	out_hitpoint_normal			= texture(buffer, vec3(uv, 2.0)).rgba;
	out_hitpoint_render			= texture(buffer, vec3(uv, 3.0)).rgba;
	out_hitpoint_color			= texture(buffer, vec3(uv, 4.0)).rgba;
	out_glossy_hitpoint_pos		= texture(buffer, vec3(uv, 5.0)).rgba;
	out_glossy_hitpoint_normal	= texture(buffer, vec3(uv, 6.0)).rgba;
	out_glossy_hitpoint_ray		= texture(buffer, vec3(uv, 7.0)).rgba;

	init_seed();

	vec3 render;
	if (rt.diffuse_bounce_count == 0 && rt.glossy_bounce_count == 0)
	{
		render = trace_ray(get_camera_ray());

		out_glossy_hitpoint_pos		= out_hitpoint_pos;
		out_glossy_hitpoint_normal	= out_hitpoint_normal;
	}
	else if (rt.glossy_bounce_count == 0)
	{
		t_hitpoint previous;
		previous.hit = bool(texture(buffer, vec3(uv, 1.0)).a);
		if (previous.hit == true)
		{
			previous.pos			= texture(buffer, vec3(uv, 1.0)).rgb;
			previous.normal			= texture(buffer, vec3(uv, 2.0)).rgb;
			previous.material_idx	= int(texture(buffer, vec3(uv, 2.0)).a);
			previous.color			= texture(buffer, vec3(uv, 4.0)).rgb;

			t_ray bounce_ray;
			bounce_ray.origin = previous.pos;
			bounce_ray.dir = bounce(previous.normal);

			render = texture(buffer, vec3(uv, 3.0)).rgb;
			render += add_bounce_light(bounce_ray, previous);
			out_hitpoint_render.a	= texture(buffer, vec3(uv, 3.0)).a;
		}
		else
		{
			render = texture(buffer, vec3(uv, 3.0)).rgb;
			out_hitpoint_pos.a = float(false); // hitpoint.hit
		}
	}
	else
	{
		t_hitpoint previous;
		previous.hit = bool(texture(buffer, vec3(uv, 5.0)).a);
		if (previous.hit == true)
		{
			previous.pos			= texture(buffer, vec3(uv, 5.0)).rgb;
			previous.normal			= texture(buffer, vec3(uv, 6.0)).rgb;
			previous.material_idx	= int(texture(buffer, vec3(uv, 6.0)).a);
			previous.ray			= texture(buffer, vec3(uv, 7.0)).rgb;

			float previous_metallic		= texture(buffer, vec3(uv, 3.0)).a;
			float previous_roughness	= texture(buffer, vec3(uv, 7.0)).a;

			vec3 specular			= vec3(0.0);
			if (rt.glossy_bounce_count > 0)
				specular = texture(buffer, vec3(uv, 4.0)).rgb;

			t_ray reflection_ray;
			reflection_ray.origin	= previous.pos;
			reflection_ray.dir		= reflect(previous.ray, previous.normal, previous_roughness);

			render = texture(buffer, vec3(uv, 3.0)).rgb;
			render += add_reflection_light(reflection_ray, previous, specular, previous_metallic, previous_roughness);
		}
		else
		{
			render = texture(buffer, vec3(uv, 3.0)).rgb;
			out_glossy_hitpoint_pos.a = float(false); // hitpoint.hit
		}
	}
	out_hitpoint_render.rgb = render;

	vec3 cumulative_render_buffer = texture(buffer, vec3(uv, 0.0)).rgb;
	if (rt.diffuse_bounce_count == rt.max_diffuse_bounces && rt.glossy_bounce_count == rt.max_glossy_bounces)
	{
		FragColor.rgb = mix(cumulative_render_buffer, render, 1.0 / rt.sample_count);
	}
	else
		FragColor.rgb = cumulative_render_buffer;
}
