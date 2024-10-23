#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 out_hitpoint_pos;				// alpha is hitpoint.hit
layout(location = 2) out vec4 out_hitpoint_normal;			// alpha is hitpoint.material_idx
layout(location = 3) out vec4 out_hitpoint_render;
layout(location = 4) out vec4 out_hitpoint_color;			// this doubles as glossy_specular_accumulation
layout(location = 5) out vec4 out_glossy_hitpoint_pos;		// alpha is hitpoint.hit
layout(location = 6) out vec4 out_glossy_hitpoint_normal;	// alpha is hitpoint.material_idx
layout(location = 7) out vec4 out_glossy_hitpoint_ray;

#import header.frag
#import AgX.frag
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

void	main(void)
{
	vec2 uv = vec2(coord.x, 1.0 - coord.y);

	g_seed = int(fract(sin(dot(vec2(coord.xy), vec2(12.9898, 78.233))) * 43758.5453123) * 5929 * (rt.sample_count + 1)) + rt.sample_count * 9823;
	g_seed += int(rand() * 943 * rt.sample_count);
	g_seed += int(rand() * 7943 * rt.diffuse_bounce_count);
	g_seed += int(rand() * 2348 * rt.glossy_bounce_count);

	vec3 render;
	if (rt.diffuse_bounce_count == 0 && rt.glossy_bounce_count == 0)
	{
		vec2 screen = (coord * 2 - 1) / vec2(1, rt.aspect_ratio);
		t_ray camera_ray;
		camera_ray.origin = rt.camera.origin;
		vec3 camera_up = cross(rt.camera.direction, rt.camera.right);
		camera_ray.dir = screen.y * camera_up + screen.x * rt.camera.right + rt.camera.focal_length * rt.camera.direction;
		camera_ray.dir.x = camera_ray.dir.x - (2.0 / rt.width  / 2.0) + (2.0 / rt.width  * rand() * 1);
		camera_ray.dir.y = camera_ray.dir.y - (2.0 / rt.height / 2.0) + (2.0 / rt.height * rand() * 1);
		render = trace_ray(camera_ray);
		out_hitpoint_render.rgb = render;

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
			out_hitpoint_render.rgb = render;
		}
		else
		{
			render = texture(buffer, vec3(uv, 3.0)).rgb;
			out_hitpoint_render.rgb = render;
			out_hitpoint_pos.a = float(false); // hitpoint.hit
		}
		// out_glossy_hitpoint_pos		= texture(buffer, vec3(uv, 5.0));
		// out_glossy_hitpoint_normal	= texture(buffer, vec3(uv, 6.0));
		// out_glossy_hitpoint_ray		= texture(buffer, vec3(uv, 7.0));
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
			vec3 specular			= vec3(0.0);
			if (rt.glossy_bounce_count > 0)
				specular = texture(buffer, vec3(uv, 4.0)).rgb;

			t_ray reflection_ray;
			reflection_ray.origin	= previous.pos;
			reflection_ray.dir		= reflect(previous.ray, previous.normal, materials[previous.material_idx].roughness);

			render = texture(buffer, vec3(uv, 3.0)).rgb;
			render += add_reflection_light(reflection_ray, previous, specular);
			out_hitpoint_render.rgb = render;
		}
		else
		{
			render = texture(buffer, vec3(uv, 3.0)).rgb;
			out_hitpoint_render.rgb = render;
			out_glossy_hitpoint_pos.a = float(false); // hitpoint.hit
		}
	}

	vec3 cumulative_render_buffer = texture(buffer, vec3(uv, 0.0)).rgb;
	if (rt.diffuse_bounce_count == rt.max_diffuse_bounces && rt.glossy_bounce_count == rt.max_glossy_bounces)
	{
		FragColor.rgb = mix(cumulative_render_buffer, render, 1.0 / rt.sample_count);
	}
	else
		FragColor.rgb = cumulative_render_buffer;
}
