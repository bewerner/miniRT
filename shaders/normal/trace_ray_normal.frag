int	next_object_type(inout int i)
{
	if (i == -1)
	{
		i++;
		return (int(texelFetch(objects, i).r));
	}
	i = int(texelFetch(objects, i + 1).r);
	return (int(texelFetch(objects, i).r));
}

bool	is_obstructed(t_ray ray)
{
	t_hitpoint	current;
	float		ray_len;

	current = HP_INF;
	ray_len = length(ray.dir);

	int i = -1;
	int type = next_object_type(i);
	while (type != OBJ_NONE)
	{
		if (type == OBJ_SPHERE)
			current = get_hitpoint_sphere(ray, get_sphere(i));
		else if (type == OBJ_PLANE)
			current = get_hitpoint_plane(ray, get_plane(i));
		else if (type == OBJ_CYLINDER)
			current = get_hitpoint_cylinder(ray, get_cylinder(i));
		if (length(current.ray) < ray_len)
			return (true);
		type = next_object_type(i);
	}
	return (false);
}

t_hitpoint	get_closest_hitpoint(t_ray ray)
{
	t_hitpoint	closest;
	t_hitpoint	current;

	closest = HP_INF;
	current = HP_INF;

	int i = -1;
	int type = next_object_type(i);
	while (type != OBJ_NONE)
	{
		if (type == OBJ_SPHERE)
			current = get_hitpoint_sphere(ray, get_sphere(i));
		else if (type == OBJ_PLANE)
			current = get_hitpoint_plane(ray, get_plane(i));
		else if (type == OBJ_CYLINDER)
			current = get_hitpoint_cylinder(ray, get_cylinder(i));
		if (length(current.ray) < length(closest.ray))
			closest = current;
		type = next_object_type(i);
	}
	return (closest);
}

vec4	trace_ray_normal(t_ray ray)
{
	t_hitpoint	hitpoint;
	vec4		col;
	vec4		col_norm;
	vec4		col_spec;
	vec4		col_refl;

	hitpoint = get_closest_hitpoint(ray);
	if (!hitpoint.hit)
		return (vec4(1, 0, 0, 1));

	col_norm = get_normal_color(hitpoint);
	// col_spec = get_specular_color(hitpoint);
	// col_norm = vec4_add(col_norm, col_spec);

	// col_norm = col_spec;
	// col_norm = vec4_scale(0.5f, vec4_add(col_norm, col_spec));

	// reflection
	// col_refl = get_reflection_color(hitpoint);
	// col_norm = vec4_scale(0.5f, vec4_add(col_norm, col_refl));		// lerp between diff - refl with metallicness factor (0.0 - 1.0)

	col = col_norm;
	return (col);
}