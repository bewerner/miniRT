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

	hitpoint = get_closest_hitpoint(ray);
	if (!hitpoint.hit)
		return (vec4(0, 0, 0, 1));

	col = vec4(hitpoint.normal.r * 0.5 + 0.5, hitpoint.normal.g * 0.5 + 0.5, hitpoint.normal.b * 0.5 + 0.5, 1.0);

	return (col);
}