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

vec3 get_random_cosine_weighted_hemisphere_direction(vec3 hemisphere_normal)
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

	// return (clamp(texture(environment_map, uv).rgb, 3, 999999) - 3);
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
// 	ray.dir = get_random_cosine_weighted_hemisphere_direction(hitpoint.normal);
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

// ivec2	get_pixel_pos(float target, int width, int height)
// {
// 	float	current = 0;
// 	ivec2	pos = ivec2(0, 0);
// 	ivec2	previous_pos;

// 	previous_pos = pos;
// 	current = texelFetch(environment_map, pos, 0).a;
// 	while (pos.y < height && current < target)
// 	{
// 		previous_pos = pos;
// 		current = texelFetch(environment_map, pos, 0).a;
// 		pos.y++;
// 	}
// 	pos = previous_pos;
// 	current = texelFetch(environment_map, pos, 0).a;
// 	while (pos.y > 0 && current > target)
// 	{
// 		previous_pos = pos;
// 		current = texelFetch(environment_map, pos, 0).a;
// 		pos.y--;
// 	}
// 	pos = previous_pos;
// 	current = texelFetch(environment_map, pos, 0).a;
// 	while (pos.x < width && current < target)
// 	{
// 		previous_pos = pos;
// 		current = texelFetch(environment_map, pos, 0).a;
// 		pos.x++;
// 	}
// 	// return (ivec2(2000, pos.y));
// 	if (current == target)
// 		return (pos);
// 	return (previous_pos);
// }

// ivec2	get_pixel_pos(float target, int width, int height)
// {
// 	float	current = 0;
// 	ivec2	pos = ivec2(0, 0);

// 	if (target <= 0)
// 		return (pos);
// 	if (target >= 1)
// 		return (ivec2(width - 1, height - 1));

// 	while (pos.y < height && current < target)
// 	{
// 		current = texelFetch(environment_map, pos, 0).a;
// 		pos.y++;
// 	}
// 	pos.y--;
// 	current = 0;
// 	while (pos.x < width && current < target)
// 	{
// 		current = texelFetch(environment_map, pos, 0).a;
// 		pos.x++;
// 	}
// 	pos.x--;
// 	return (pos);
// }

// ivec2 get_pixel_pos(float target, int width, int height)
// {
// 	float current = 0.0;
// 	for (int y = 0; y < height; ++y)
// 	{
// 		for (int x = 0; x < width; ++x)
// 		{
// 			current = texelFetch(environment_map, ivec2(x, y), 0).a;
// 			if (current >= target)
// 				return ivec2(x, y);
// 		}
// 	}
// 	return ivec2(-1, -1); // Return invalid pixel position if target is not found
// }

// // Function to retrieve the pixel position based on a random value and the CDF using binary search
// vec2 get_pixel_pos(float target, int width, int height)
// {
// 	int		total_pixels = width * height;
// 	vec2	lower = vec2(1.0 / width / 2, 1.0 / height / 2);
// 	vec2	middle;
// 	vec2	upper = vec2(1) - lower;
// 	float	cdf;
// 	vec2	uv;

// 	// Loop to perform binary search
// 	while (upper.y > lower.y || (upper.y == lower.y && upper.x > lower.x))
// 	{
// 		middle = (lower + upper) / 2;

// 		// Calculate the x and y coordinates from the middle index
// 		// uv.x = middle % width;
// 		// uv.y = middle / width;

// 		// Sample the environment map at the middle pixel position
// 		cdf = texture(environment_map, middle).a;
// 		if (cdf < target)
// 			lower = middle + 1;
// 		else if (cdf > target)
// 			upper = middle - 1;
// 		else if (cdf == target)
// 			return (middle);
// 	}
// 	return (middle); // Return the pixel coordinates as an ivec2

// // 	// At this point, lower is the first index where the CDF >= target
// // 	// Ensure lower is within bounds
// // 	if (lower < total_pixels) {
// // 		uv.x = lower % width;
// // 		uv.y = lower / width;
// // 		return (uv); // Return the pixel coordinates as an ivec2
// // 	}

// // 	// Fallback return value (should not reach here if target is valid)
// // 	return ivec2(-1, -1); // Return invalid pixel position if target is not found
// }

// // Function to retrieve the pixel position based on a random value and the CDF using binary search
// ivec2 get_pixel_pos(float target, int width, int height)
// {
// 	int		total_pixels = width * height;
// 	int		lower = 0;
// 	int		middle;
// 	int		upper = total_pixels - 1;
// 	float	cdf;
// 	ivec2	pos;

// 	// Loop to perform binary search
// 	while (lower <= upper)
// 	{
// 		middle = (lower + upper) / 2;

// 		// Calculate the x and y coordinates from the middle index
// 		pos.x = middle % width;
// 		pos.y = middle / width;

// 		// Sample the environment map at the middle pixel position
// 		cdf = texelFetch(environment_map, pos, 0).a;
// 		if (cdf < target)
// 			lower = middle + 1;
// 		else if (cdf > target)
// 			upper = middle - 1;
// 		else if (cdf == target)
// 			return (pos);
// 	}
// 	return (pos); // Return the pixel coordinates as an ivec2

// 	// At this point, lower is the first index where the CDF >= target
// 	// Ensure lower is within bounds
// 	if (lower < total_pixels) {
// 		pos.x = lower % width;
// 		pos.y = lower / width;
// 		return (pos); // Return the pixel coordinates as an ivec2
// 	}

// 	// Fallback return value (should not reach here if target is valid)
// 	return ivec2(-1, -1); // Return invalid pixel position if target is not found
// }

// Function to retrieve the pixel position based on a random value and the CDF using binary search
ivec2 get_pixel_pos(float target, int width, int height)
{
	int		total_pixels = width * height;
	int		lower = 0;
	int		middle;
	int		upper = total_pixels - 1;
	float	cdf;
	ivec2	pos;

	// Loop to perform binary search
	while (lower <= upper)
	{
		middle = (lower + upper) / 2;

		// Calculate the x and y coordinates from the middle index
		pos.x = middle % width;
		pos.y = middle / width;

		// Sample the environment map at the middle pixel position
		cdf = texture(environment_map, vec2(float(pos.x) / width, float(pos.y) / height), 0).a;
		if (cdf < target)
			lower = middle + 1;
		else if (cdf > target)
			upper = middle - 1;
		else if (cdf == target)
			return (pos);
	}
	return (pos); // Return the pixel coordinates as an ivec2

	// At this point, lower is the first index where the CDF >= target
	// Ensure lower is within bounds
	if (lower < total_pixels) {
		pos.x = lower % width;
		pos.y = lower / width;
		return (pos); // Return the pixel coordinates as an ivec2
	}

	// Fallback return value (should not reach here if target is valid)
	return ivec2(-1, -1); // Return invalid pixel position if target is not found
}

float	get_pdf(ivec2 pos, int width, int height)
{
	float weight = texelFetch(environment_map, pos, 0).a;
	pos.x--;
	if (pos.x < 0)
	{
		pos.y--;
		pos.x = width - 1;
	}
	if (pos.y < 0)
		return (weight);
	return (weight - texelFetch(environment_map, pos, 0).a);
}

vec3	get_random_importance_weighted_direction(out float pdf)
{
	int		width = 2048;
	int		height = 1024;
	float	random_value;
	vec3	direction;
	ivec2	pixel_pos;
	vec2	uv;

	random_value = rand();
	// random_value = rand() * 1000;
	// random_value = 658;
	pixel_pos = get_pixel_pos(random_value, width, height);
	pdf = get_pdf(pixel_pos, width, height);
	// pdf /= 1000;

	// uv.x = (float(pixel_pos.x + 1)) / width;
	// uv.y = (float(pixel_pos.y + 1)) / height;
	uv.x = (float(pixel_pos.x + 1) + (rand() - 0.5)) / width;
	uv.y = (float(pixel_pos.y + 1) + (rand() - 0.5)) / height;
	uv = clamp(uv, 0, 1);

	float theta = (uv.x - 0.5) * (2.0 * M_PI);
	float phi = (0.5 - uv.y) * (M_PI);
	float cos_phi = cos(phi);
	direction.x = cos_phi * cos(theta);
	direction.y = -cos_phi * sin(theta);
	direction.z = -sin(phi);

	return (direction);
}

vec3	get_ambient_color(t_hitpoint hitpoint)
{
	vec3	col = VEC3_BLACK;
	t_ray	ray;
	
	ray.origin = get_offset_hitpoint_pos(hitpoint);
	ray.dir = get_random_cosine_weighted_hemisphere_direction(hitpoint.normal);
	if (reaches_sky(ray) == true)
		col = rt.ambient;
	return (col);
}

vec3	get_sky_color(t_hitpoint hitpoint)
{
	if (rt.ambient.r >= 0)
		return (get_ambient_color(hitpoint));

	vec3	col_importance = VEC3_BLACK;
	vec3	col_cosine = VEC3_BLACK;
	vec3	col_final;
	t_ray	ray_importance;
	t_ray	ray_cosine;
	float	weight_importance;
	float	weight_cosine;
	float	pdf_importance;
	float	pdf_cosine;

	ray_importance.origin = get_offset_hitpoint_pos(hitpoint);
	ray_cosine.origin = ray_importance.origin;
	ray_importance.dir = get_random_importance_weighted_direction(pdf_importance);
	ray_cosine.dir = get_random_cosine_weighted_hemisphere_direction(hitpoint.normal);

	pdf_cosine = cos(dot(hitpoint.normal, ray_cosine.dir)) / M_PI;
	pdf_cosine = max(pdf_cosine, 1e-6);
	// if (pdf_importance <= 1e-6)
	// 	return(vec3(1,0,0));
	pdf_importance = max(pdf_importance, 1e-6);

	weight_importance = pdf_importance / (pdf_importance + pdf_cosine);
	weight_cosine = pdf_cosine / (pdf_cosine + pdf_importance);

	if (reaches_sky(ray_importance) == true)
		col_importance = get_environment_map_color(ray_importance.dir) / (((50000))) * max(0, dot(hitpoint.normal, ray_importance.dir)) / pdf_importance;
	if (reaches_sky(ray_cosine) == true)
		col_cosine = clamp(get_environment_map_color(ray_cosine.dir) / pdf_cosine, 0, 16);

	col_final = col_importance * weight_importance + (col_cosine * (((0.3)))) * weight_cosine;
	// if (rt.debug == 1)
	// 	col_final = mix(col_importance, col_cosine, weight_cosine);

	// return (col_importance);
	// return (clamp(col_importance, 0, 1));
	// return (col_importance / 10000000);
	// if (rt.debug == 1)
	// 	return (col_cosine * pdf_cosine);
	return (col_final);
}

vec3	get_sky_color_from_ray(t_ray ray, t_hitpoint hitpoint)
{
	vec3	col = VEC3_BLACK;

	if (hitpoint.hit == false)
	{
		if (rt.ambient.r >= 0)
			col = rt.ambient;
		else
			col = get_environment_map_color(ray.dir);
	}
	return (col);
}
