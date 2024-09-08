float	RandomValueNormalDistribution(void)
{
	// Thanks to https://stackoverflow.com/a/6178290
	float theta = 2 * 3.1415926 * rand();
	float rho = sqrt(-2 * log(rand()));
	return (rho * cos(theta));
}

bool	reaches_sky(t_ray ray)
{
	t_hitpoint	hitpoint = HP_INF;

	int i = -1;
	int type = next_object_type(i);
	while (type != OBJ_NONE)
	{
		if (type == OBJ_SPHERE)
			hitpoint = get_hitpoint_sphere(ray, get_sphere(i));
		else if (type == OBJ_PLANE)
			hitpoint = get_hitpoint_plane(ray, get_plane(i));
		else if (type == OBJ_CYLINDER)
			hitpoint = get_hitpoint_cylinder(ray, get_cylinder(i));
		else if (type == OBJ_HYPERBOLOID)
			hitpoint = get_hitpoint_hyperboloid(ray, get_hyperboloid(i));
		if (hitpoint.hit == true)
			return (false);
		type = next_object_type(i);
	}
	return (true);
}

vec3	get_random_hemisphere_direction(vec3 hemisphere_normal)
{
	vec3	direction;

	if (rt.debug == 2)
		direction = vec3(RandomValueNormalDistribution(), RandomValueNormalDistribution(), RandomValueNormalDistribution());
	else
		direction = vec3(rand() - 0.5, rand() - 0.5, rand() - 0.5);
	if (dot(direction, hemisphere_normal) <= 0)
		direction *= -1;
	return (direction);
}

vec4	get_sky_color(t_hitpoint hitpoint)
{
	if (1 == 1)
		return (rt.ambient);
	vec4	col = VEC4_BLACK;
	t_ray	ray;
	// int samples = int(rt.debug) * 10;
	int samples = 128;

	ray.origin = hitpoint.pos + hitpoint.normal * 0.0001;
	for (int i = 0; i < samples; i++)
	{
		ray.dir = get_random_hemisphere_direction(hitpoint.normal);
		if (reaches_sky(ray) == true)
			col += rt.ambient;
	}
	col /= samples;
	col.a = 1.0;
	return (col);
}
