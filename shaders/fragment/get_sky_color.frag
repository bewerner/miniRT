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

	return (texture(environment_map, uv).rgb);
	// return (texture(environment_map, uv).rgb);
	// return(clamp(texture(environment_map, uv).rgb, 0, 16));
}

// vec3	get_sky_color(t_hitpoint hitpoint)
// {
// 	vec3	col = VEC3_BLACK;
// 	// vec3	col = vec3(1,0,0);
// 	t_ray	ray;

// 	ray.origin = get_offset_hitpoint_pos(hitpoint);
// 	ray.dir = get_random_hemisphere_direction(hitpoint.normal);
// 	if (reaches_sky(ray) == true)
// 	{
// 		// if (rt.debug == -1)
// 		// 	col = get_environment_map_color(ray.dir);
// 		// else
// 		// 	col = rt.ambient;
// 		if (rt.ambient.r >= 0)
// 			col = rt.ambient;
// 		else
// 			col = get_environment_map_color(ray.dir);
// 	}
// 	return (col);
// }

ivec2	get_pixel_pos(float target, int width, int height)
{
	float	current = 0;
	ivec2	pos = ivec2(0, 0);
	ivec2	previous_pos;

	while (pos.y < height && current < target)
	{
		previous_pos = pos;
		current = texelFetch(environment_map, pos, 0).a;
		pos.y++;
	}
	if (current == target)
		return (pos);
	pos = previous_pos;
	while (pos.x < width && current < target)
	{
		previous_pos = pos;
		current = texelFetch(environment_map, pos, 0).a;
		pos.x++;
	}
	if (current == target)
		return (pos);
	return (previous_pos);
}

vec3	get_random_importance_weighted_direction(out float weight)
{
	int		width = 2048;
	int		height = 1024;
	float	random_value;
	vec3	direction;
	ivec2	pixel_pos;
	vec2	uv;

	random_value = rand();
	// random_value = 0.5;
	pixel_pos = get_pixel_pos(random_value, width, height);
	weight = texelFetch(environment_map, pixel_pos, 0).a - texelFetch(environment_map, pixel_pos - ivec2(1, 1), 0).a;

	uv.x = float(pixel_pos.x + 1) / width;
	uv.y = float(pixel_pos.y + 1) / height;

	// float theta = uv.y * (M_PI);
	// float phi = uv.x * (2 * M_PI);
	// float sin_theta = sin(theta);
	// direction.x = sin_theta * cos(phi);
	// direction.y = sin_theta * sin(phi);
	// direction.z = cos(theta);

	float theta = (uv.x - 0.5) * (2.0 * M_PI);
	float phi = (0.5 - uv.y) * (M_PI);
	// float sin_theta = sin(theta);
	direction.x = cos(phi) * cos(theta);
	direction.y = -cos(phi) * sin(theta);
	direction.z = -sin(phi);
	// direction = normalize(direction);

	// float theta = (uv.x - 0.5) * (2 * M_PI);
	// float phi = (0.5 - uv.y) * (M_PI);
	// float sin_phi = sin(phi);
	// direction.x = sin_phi * cos(theta);
	// direction.y = sin_phi * sin(theta);
	// direction.z = cos(phi);

	// if (dot(direction, normal) < 0)
	// 	direction *= -1;
	// return (vec3(float(pixel_pos.x) / width, float(pixel_pos.y) / height, 0));
	// if (pixel_pos.y > -1)
	// 	return (vec3(1));
	// return (vec3(0));
	if (rt.debug == -1)
		return (vec3(texelFetch(environment_map, pixel_pos, 0).a));
	if (rt.debug == -2)
		return (vec3(texture(environment_map, vec2(0.8,0.8)).a));
	return (direction);
}

vec3	get_sky_color(t_hitpoint hitpoint)
{
	vec3	col = VEC3_BLACK;
	// vec3	col = vec3(1,0,0);
	t_ray	ray;
	float	weight;

	ray.origin = get_offset_hitpoint_pos(hitpoint);
	ray.dir = get_random_importance_weighted_direction(weight);
	if (dot(ray.dir, hitpoint.normal) >= 0 && reaches_sky(ray) == true)
	{
		// if (rt.debug == -1)
		// 	col = get_environment_map_color(ray.dir);
		// else
		// 	col = rt.ambient;
		if (rt.ambient.r >= 0)
			col = rt.ambient;
		else
			col = get_environment_map_color(ray.dir) / weight;
			// col = ray.dir;
			// col = vec3(weight);
	}
	return (col);
}
