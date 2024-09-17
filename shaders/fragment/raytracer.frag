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
	vec2 tmp = vec2(coord.x, 1.0 - coord.y);
	// vec2 tmp = coord;

	g_seed = int(fract(sin(dot(vec2(coord.xy), vec2(12.9898, 78.233))) * 43758.5453123) * 5929 * (rt.frame + 1)) + rt.frame * 9823;
	g_seed += int(rand() * 943 * rt.frame);

	if (rt.frame >= rt.max_samples)
	{
		vec3 col = texture(prevFrameTexture, tmp).rgb;
		if (rt.frame < rt.max_samples)
		col = to_agx(col.rgb);
		col = dither(col);
		FragColor = col;
		return ;
	}
	if (rt.debug == -1 || rt.frame >= rt.max_samples)
	{
		vec3 col = texture(prevFrameTexture, tmp).rgb;
		if (rt.frame < rt.max_samples)
		col = to_agx(col.rgb);
		col = dither(col);
		FragColor = col;
		// FragColor = vec3(tmp , 0);
		// FragColor = mix(vec3(0,0,0), texture(prevFrameTexture, tmp).rgb, 0.1);
		return ;
	}
	if (rt.debug == -2)
	{
		// FragColor = vec3(tmp , 0);
		// vec3 col = texture(prevFrameTexture, tmp).rgb;
		// col = to_agx(col.rgb);
		// col = dither(col);
		// FragColor = col;
		// FragColor = mix(vec3(0,0,0), texture(prevFrameTexture, tmp).rgb, 0.99);
		return ;
	}

	t_ray camera_ray;
	camera_ray.origin = rt.camera.origin;
	vec3 camera_up = cross(rt.camera.direction, rt.camera.right);
	camera_ray.dir = uv.y * camera_up + uv.x * rt.camera.right + rt.camera.focal_length * rt.camera.direction;
	camera_ray.dir.x = camera_ray.dir.x - (2.0 / 1280 / 2.0) + (2.0 / 1280 * rand() * 1);
	camera_ray.dir.y = camera_ray.dir.y - (2.0 /  720 / 2.0) + (2.0 /  720 * rand() * 1);

	vec3 col;
	col = trace_ray(camera_ray);
	// col = to_agx(col.rgb);
	// col = dither(col);

	FragColor = col;
	// col = vec3(rand());
	// if (uv.x > 0.9)
	// 	FragColor = texture(prevFrameTexture, vec2(0.5, 0.5)).rgb;
	// 	// FragColor = texture(prevFrameTexture, uv).rgb;

	if (rt.frame > 0)
		FragColor = mix(texture(prevFrameTexture, tmp).rgb, col, 1.0 / rt.frame);
		// FragColor = mix(col, texture(prevFrameTexture, tmp).rgb, 0.5);
		// FragColor = texture(prevFrameTexture, tmp).rgb;
		// FragColor = vec3(1,0,0);
	// FragColor = (col + texture(prevFrameTexture, tmp).rgb) / 2;
}
