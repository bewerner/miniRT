
vec4	make_mul_col(vec4 col)
{
	col.r = max(col.r, .3);
	col.g = max(col.g, .3);
	col.b = max(col.b, .3);
	return (col);
}

vec4	shoot_ray(t_ray ray, inout t_hitpoint hitpoint, inout vec4 mult_col, float bounce_value)
{
	// t_hitpoint	hitpoint;
	vec4		col;

	// mult_col += hitpoint.color * (1 - bounce_value) * (bounce_value);
	mult_col *= make_mul_col(hitpoint.color);
	// mult_col *= (1 - bounce_value);
	hitpoint = get_closest_hitpoint(ray);
	if (!hitpoint.hit)
		return (rt.ambient * mult_col);

	col = get_diffuse_color(hitpoint) * get_illumination_color(hitpoint);

	return (col * mult_col);
}


vec4	get_reflection_color(t_hitpoint hitpoint)
{
	vec4	col;
	t_ray	bounce_ray;
	int		MAX_BOUNCES = 10;
	int		bounce_count = 0;
	// float	bounce_value = g_metallic;
	float	bounce_value = materials[hitpoint.material_idx].metallic;
	vec4	mult_col = make_mul_col(hitpoint.color);

	col = VEC4_BLACK;
	// mult_col = VEC4_BLACK;
	// mult_col = vec4(1,1,1,1);
	// if (rt.debug > 0)
	// 	bounce_value = 1;

	while (bounce_count < MAX_BOUNCES && hitpoint.hit == true && materials[hitpoint.material_idx].metallic > 0.0)
	{
		bounce_ray.origin = hitpoint.pos + 0.001 * hitpoint.normal;
		bounce_ray.dir = create_bounce_dir(hitpoint.ray, hitpoint.normal);
		
		col += shoot_ray(bounce_ray, hitpoint, mult_col, bounce_value) * (1 - bounce_value) * bounce_value;
		// col += shoot_ray(bounce_ray, hitpoint, mult_col) * (1 - bounce_value);
		// col += shoot_ray(bounce_ray, hitpoint, mult_col) * bounce_value;
		bounce_value *= materials[hitpoint.material_idx].metallic;
		bounce_count++;
	}
	return (col);
}
