vec3	create_bounce_dir(vec3 incoming_dir, vec3 normal)
{
	return (incoming_dir - (2.0 * dot(incoming_dir, normal) * normal));
}

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
		else if (type == OBJ_HYPERBOLOID)
			current = get_hitpoint_hyperboloid(ray, get_hyperboloid(i));
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
		else if (type == OBJ_HYPERBOLOID)
			current = get_hitpoint_hyperboloid(ray, get_hyperboloid(i));
		if (length(current.ray) < length(closest.ray))
			closest = current;
		type = next_object_type(i);
	}
	return (closest);
}

vec4	trace_ray(t_ray ray)
{
	t_hitpoint	hitpoint;
	vec4		col;
	vec4		col_illumination = VEC4_BLACK;
	vec4		col_sky = VEC4_BLACK;
	vec4		col_diffuse = VEC4_BLACK;
	vec4		col_specular = VEC4_BLACK;
	vec4		col_reflection = vec4(1,1,1,1);

	int bounce_count = 0;

	// while (bounce_count <= 0)
	// {
		hitpoint = get_closest_hitpoint(ray);
		if (!hitpoint.hit)
			return (rt.ambient);

		col_illumination = get_illumination_color(hitpoint);
		col_sky = get_sky_color(hitpoint);
		col_diffuse = get_diffuse_color(hitpoint);
		col_specular = get_specular_color(hitpoint, col_illumination);
		col_reflection = get_reflection_color(hitpoint);


	// 	ray.origin = hitpoint.pos + 0.001 * hitpoint.normal;
	// 	ray.dir = create_bounce_dir(hitpoint.ray, hitpoint.normal);

	// 	bounce_count++;
	// }

	col = col_diffuse + col_specular * 0.0;
	// col = (1.0 - g_metallic) * (col_diffuse + col_specular) + g_metallic * col_reflection;
	// col = (1.0 - g_metallic) * (1.0 - g_metallic) * col_diffuse + g_metallic * col_reflection;

	// original::
	// col = (1.0 - g_metallic) * col_diffuse + g_metallic * col_reflection;
	// WITH MATERIAL ::
	col = (1.0 - materials[hitpoint.material_idx].metallic) * col_diffuse * (col_illumination + col_sky) + materials[hitpoint.material_idx].metallic * col_reflection;

	return (col);
}
