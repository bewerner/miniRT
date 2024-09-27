vec3	create_bounce_dir(vec3 incoming_dir, vec3 normal)
{
	return (incoming_dir - (2.0 * dot(incoming_dir, normal) * normal));
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
			current = get_hitpoint_plane(ray, get_plane(i), false);
		else if (type == OBJ_CYLINDER)
			current = get_hitpoint_cylinder(ray, get_cylinder(i));
		else if (type == OBJ_HYPERBOLOID)
			current = get_hitpoint_hyperboloid(ray, get_hyperboloid(i));
		if (length(current.ray) < ray_len)
			return (true);
		type = next_object_type(i);
	}
	return (false);
}
