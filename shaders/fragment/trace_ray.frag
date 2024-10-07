vec3	trace_ray(t_ray ray)
{
	t_hitpoint	hitpoint;
	vec3		col;
	vec3		col_illumination = VEC3_BLACK;
	vec3		col_sky = VEC3_BLACK;
	vec3		col_diffuse = VEC3_BLACK;
	vec3		col_specular = VEC3_BLACK;
	vec3		col_reflection = VEC3_WHITE;

	int bounce_count = 0;

	// while (bounce_count <= 0)
	// {
		hitpoint = get_closest_hitpoint(ray, true);
		if (!hitpoint.hit)
		{
			// if (rt.debug == -1)
			// 	return(get_environment_map_color(ray.dir));
			// else
			// 	return (rt.ambient);
			if (rt.ambient.r >= 0)
				return (rt.ambient);
			else
				return (get_environment_map_color(ray.dir));
		}

		hitpoint.color = get_hitpoint_color(hitpoint);

		col_illumination = get_illumination_color(hitpoint);
		col_sky = get_sky_color(hitpoint);
		col_diffuse = get_diffuse_color(hitpoint);
		col_specular = get_specular_color(hitpoint, col_illumination);
		col_reflection = get_reflection_color(hitpoint);


	// 	ray.origin = hitpoint.pos + 0.001 * hitpoint.normal;
	// 	ray.dir = create_bounce_dir(hitpoint.ray, hitpoint.normal);

	// 	bounce_count++;
	// }

	// col = col_diffuse + col_specular * 0.0;

	// col = (1.0 - g_metallic) * (col_diffuse + col_specular) + g_metallic * col_reflection;
	// col = (1.0 - g_metallic) * (1.0 - g_metallic) * col_diffuse + g_metallic * col_reflection;

	// original::
	// col = (1.0 - g_metallic) * col_diffuse + g_metallic * col_reflection;
	// WITH MATERIAL ::
	col = (1.0 - materials[hitpoint.material_idx].metallic) * col_diffuse * (col_illumination + col_sky)
	+ col_specular * 1.0
	+ materials[hitpoint.material_idx].metallic * col_reflection;

	// return (get_checker_color(hitpoint));
	// return (col_diffuse);
	return (col);
}
