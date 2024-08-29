vec4	shoot_ray(t_ray ray, inout t_hitpoint hitpoint)
{
	// t_hitpoint	hitpoint;
	vec4		col_diff;

	vec4 hitpoint_color = hitpoint.color;
	hitpoint = get_closest_hitpoint(ray);
	if (!hitpoint.hit)
		return (rt.ambient * hitpoint_color);

	col_diff = get_diffuse_color(hitpoint);

	return (col_diff * hitpoint_color);
}


vec4	get_reflection_color(t_hitpoint hitpoint)
{
	vec4	col;
	vec4	col_refl;
	t_ray	bounce_ray;

	bounce_ray.origin = hitpoint.pos + 0.001f * hitpoint.normal;
	bounce_ray.dir = create_bounce_dir(hitpoint.ray, hitpoint.normal);

	col = shoot_ray(bounce_ray, hitpoint);

	int bounce_count = 1;
	while (bounce_count < 4)
	{
		bounce_ray.origin = hitpoint.pos + 0.001 * hitpoint.normal;
		bounce_ray.dir = create_bounce_dir(hitpoint.ray, hitpoint.normal);
		bounce_count++;

		col_refl = shoot_ray(bounce_ray, hitpoint);
		if (bounce_count == 4)
			col = col * (1 - g_metallic) + col_refl * g_metallic;
		else
			col = col * (1 - g_metallic) + col_refl * g_metallic * g_metallic;
	}

	// include the base_color as meteallic materials do it like this is it mul, or add???

	return (col);
}
