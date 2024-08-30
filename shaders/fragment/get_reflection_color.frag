vec4	shoot_ray(t_ray ray, inout t_hitpoint hitpoint, inout vec4 mult_col)
{
	// t_hitpoint	hitpoint;
	vec4		col_diff;

	// mult_col = hitpoint.color;
	mult_col *= hitpoint.color;
	hitpoint = get_closest_hitpoint(ray);
	if (!hitpoint.hit)
		return (rt.ambient * mult_col);

	col_diff = get_diffuse_color(hitpoint);

	return (col_diff * mult_col);
}


vec4	get_reflection_color(t_hitpoint hitpoint)
{
	vec4	col;
	t_ray	bounce_ray;
	int		MAX_BOUNCES = 10;
	int		bounce_count = 0;
	float	bounce_value = g_metallic;
	vec4	mult_col = hitpoint.color;

	col = VEC4_BLACK;

	while (bounce_count < MAX_BOUNCES && hitpoint.hit == true)
	{
		bounce_ray.origin = hitpoint.pos + 0.001 * hitpoint.normal;
		bounce_ray.dir = create_bounce_dir(hitpoint.ray, hitpoint.normal);
		
		col += shoot_ray(bounce_ray, hitpoint, mult_col) * (1 - bounce_value);

		bounce_value *= g_metallic;
		bounce_count++;
	}
	return (col);
}
