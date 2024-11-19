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


	if (previous_hit == true)
	{
		init_seed();

		t_hitpoint previous = get_previous_hitpoint(uv);

		float previous_metallic  = get_hitpoint_metallic(previous);
		float previous_roughness = get_hitpoint_roughness(previous);

		vec3 specular = out_specular.rgb;

		t_ray reflection_ray;
		reflection_ray.origin	= get_offset_hitpoint_pos(previous);
		reflection_ray.dir		= reflect(previous.ray, previous.normal, previous.object_normal, previous_roughness);

		out_previous_ray_origin.rgb = reflection_ray.origin;
		out_previous_ray_dir.rgb = reflection_ray.dir;

		out_render.rgb += trace_reflection_ray(reflection_ray, previous, specular, previous_metallic, previous_roughness);
	}
}
