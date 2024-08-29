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
	t_ray	bounce_ray;
	int		MAX_BOUNCES = 10;
	int		bounce_count = 0;
	float	bounce_value = g_metallic;

	col = VEC4_BLACK;

	while (bounce_count < MAX_BOUNCES && hitpoint.hit)
	{
		bounce_ray.origin = hitpoint.pos + 0.001 * hitpoint.normal;
		bounce_ray.dir = create_bounce_dir(hitpoint.ray, hitpoint.normal);
		
		col += shoot_ray(bounce_ray, hitpoint) * bounce_value;

		bounce_value *= g_metallic;
		bounce_count++;
	}
	return (col);
}
