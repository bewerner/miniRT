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
			hitpoint = get_hitpoint_plane(ray, get_plane(i), false);
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

vec3 get_random_hemisphere_direction(vec3 hemisphere_normal)
{
	// Generate random numbers in the range [0, 1]
	float u = rand();
	float v = rand();

	// Convert random numbers to spherical coordinates
	float theta = acos(sqrt(u)); // Cosine weighted
	float phi = 2.0 * M_PI * v; // Uniformly distributed in [0, 2π]

	// Spherical to Cartesian coordinates
	float x = sin(theta) * cos(phi);
	float y = sin(theta) * sin(phi);
	float z = cos(theta);

	// Create a local tangent basis for the hemisphere
	vec3 tangent;
	if (abs(hemisphere_normal.y) != 1)
		tangent = normalize(cross(vec3(0, 1, 0), hemisphere_normal));
	else
		tangent = normalize(cross(vec3(1, 0, 0), hemisphere_normal));
	vec3 bitangent = cross(hemisphere_normal, tangent);

	// Convert local space coordinates to world space
	vec3 random_direction = x * tangent + y * bitangent + z * hemisphere_normal;

	return (normalize(random_direction));
}

vec3	get_environment_map_color(vec3 direction)
{
	vec2	uv;

	vec3 normal = normalize(direction);
	uv.x = 0.5 + atan(-normal.y, normal.x) / (2.0 * M_PI);
	uv.y = 0.5 - asin(-normal.z) / M_PI;

	// return (texture(environment_map, uv).rgb);
	return(clamp(texture(environment_map, uv).rgb, 0, 16));
}

vec3	get_sky_color(t_hitpoint hitpoint)
{
	vec3	col = VEC3_BLACK;
	// vec3	col = vec3(1,0,0);
	t_ray	ray;

	ray.origin = get_offset_hitpoint_pos(hitpoint);
	ray.dir = get_random_hemisphere_direction(hitpoint.normal);
	if (reaches_sky(ray) == true)
	{
		// if (rt.debug == -1)
		// 	col = get_environment_map_color(ray.dir);
		// else
		// 	col = rt.ambient;
		if (rt.ambient.r >= 0)
			col = rt.ambient;
		else
			col = get_environment_map_color(ray.dir);
	}
	return (col);
}
