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

vec4	trace_ray_solid(t_ray ray)
{
	t_hitpoint	hitpoint;
	vec4		col;
	vec4		col_diff = VEC4_BLACK;
	vec4		col_spec = VEC4_BLACK;
	vec4		solid_ambient = vec4(0.25, 0.25, 0.25, 1);

	int bounce_count = 0;

	hitpoint = get_closest_hitpoint(ray);
	if (!hitpoint.hit)
		return (solid_ambient);

	t_point_light light01;
	light01.origin = hitpoint.pos + rt.camera.viewport_light * 4.0;
	light01.color = vec4(0.8, 0.8, 0.8, 1.0);
	light01.ratio = 5.0;
	light01.power = 8.0;
	light01.intensity = 2.5;

	t_point_light light02;
	light02.origin = hitpoint.pos + rt.camera.viewport_light * -8.0;
	light02.color = vec4(0.8, 0.7, 0.6, 1.0);
	light02.ratio = 15.0;
	light02.power = 500.0;
	light02.intensity = 50;

	col_diff = get_solid_color(hitpoint);
	col_spec = get_solid_specular_color(light01, 20.0, hitpoint);
	col_spec += get_solid_specular_color(light02, 1.1, hitpoint) * 0.5;


	col = col_diff + col_spec;
	// col = (1.0 - g_metallic) * (col_diff + col_spec) + g_metallic * col_refl;
	// col = (1.0 - g_metallic) * col_diff;


	// Add Fresnel outline
	// Actually Fresnel is wrong and it should be:
	// "silhouette highlighting" or "silhouette edge highlighting"
	// float fresnel_intensity = 1.0;
	// float  fresnel = (abs(dot(ray.dir, hitpoint.normal)) * 0.5 + 0.5) * fresnel_intensity;
	// col *= fresnel;
	// col = vec4(fresnel,fresnel,fresnel,1); // DEBUG: Display Fresnel Color

	return (col);
}