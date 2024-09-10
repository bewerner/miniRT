vec4	trace_ray(t_ray ray)
{
	t_hitpoint	hitpoint;
	vec4		col;
	vec4		col_illumination = VEC4_BLACK;
	vec4		col_sky = VEC4_BLACK;
	vec4		col_diffuse = VEC4_BLACK;
	vec4		col_specular = VEC4_BLACK;
	vec4		col_reflection = vec4(1,1,1,1);

	int bounce_count = 0;

	// while (bounce_count <= 0)
	// {
		hitpoint = get_closest_hitpoint(ray);
		if (!hitpoint.hit)
			return (rt.ambient);

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
	col = (1.0 - materials[hitpoint.material_idx].metallic) * col_diffuse * (col_illumination + col_sky) + materials[hitpoint.material_idx].metallic * col_reflection;

	return (col);
}
