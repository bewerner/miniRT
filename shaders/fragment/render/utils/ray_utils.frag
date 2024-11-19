vec3	mirror(vec3 incoming, vec3 normal)
{
	return (normalize(incoming - (2.0 * dot(incoming, normal) * normal)));
}

vec3	reflect(vec3 incoming, vec3 N, vec3 object_normal, float roughness)
{
	incoming = normalize(incoming);
	vec3 reflection = -object_normal;

	for (int i = 0; dot(reflection, object_normal) < 0 && i < 32; i++)
	{
		vec3 H = sample_visible_normal(-incoming, N, roughness);
		reflection = reflect(incoming, H);
	}
	for (int i = 0; dot(reflection, object_normal) < 0 && i < 32; i++)
	{
		vec3 H = sample_visible_normal(-incoming, object_normal, roughness);
		reflection = reflect(incoming, H);
	}
	if (dot(reflection, object_normal) < 0.0)
		reflection = reflect(incoming, object_normal);

	return (reflection);
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
			current = get_hitpoint_sphere(ray, get_sphere(i), false);
		else if (type == OBJ_PLANE)
			current = get_hitpoint_plane(ray, get_plane(i), false);
		else if (type == OBJ_CYLINDER)
			current = get_hitpoint_cylinder(ray, get_cylinder(i), false);
		else if (type == OBJ_HYPERBOLOID)
			current = get_hitpoint_hyperboloid(ray, get_hyperboloid(i), false);
		if (length(current.ray) < ray_len)
			return (true);
		type = next_object_type(i);
	}
	return (false);
}

bool	reaches_sky(t_ray ray)
{
	t_hitpoint	hitpoint = HP_INF;

	int i = -1;
	int type = next_object_type(i);
	while (type != OBJ_NONE)
	{
		if (type == OBJ_SPHERE)
			hitpoint = get_hitpoint_sphere(ray, get_sphere(i), false);
		else if (type == OBJ_PLANE)
			hitpoint = get_hitpoint_plane(ray, get_plane(i), false);
		else if (type == OBJ_CYLINDER)
			hitpoint = get_hitpoint_cylinder(ray, get_cylinder(i), false);
		else if (type == OBJ_HYPERBOLOID)
			hitpoint = get_hitpoint_hyperboloid(ray, get_hyperboloid(i), false);
		if (hitpoint.hit == true)
			return (false);
		type = next_object_type(i);
	}
	return (true);
}
