vec3	get_offset_hitpoint_pos(t_hitpoint hitpoint)
{
	return (hitpoint.pos + (max(10, length(hitpoint.ray)) * 0.0001) * hitpoint.normal);
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
		else if (type == OBJ_HYPERBOLOID)
			current = get_hitpoint_hyperboloid(ray, get_hyperboloid(i));
		if (length(current.ray) < length(closest.ray))
			closest = current;
		type = next_object_type(i);
	}
	return (closest);
}
