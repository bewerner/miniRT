ivec2 sample_environment_cdf(int width, int height)
{
	float	target = rand();
	int		total_pixels = width * height;
	int		lower = 0;
	int		middle;
	int		upper = total_pixels - 1;
	float	cdf;
	float	delta;
	float	best_delta = 2;
	ivec2	pos;
	ivec2	best_pos;

	while (lower <= upper)
	{
		middle = (lower + upper) / 2;

		pos.x = middle % width;
		pos.y = middle / width;

		cdf = texelFetch(environment_map, ivec3(pos, 1), 0).r;

		delta = abs(cdf - target);
		if (delta < best_delta)
		{
			best_delta = delta;
			best_pos = pos;
		}

		if (cdf < target)
			lower = middle + 1;
		else if (cdf > target)
			upper = middle - 1;
		else if (cdf == target)
			break ;
	}

	return (best_pos);
}

vec3	sample_environment_map(out float pdf)
{
	pdf = 0;
	if (rt.ambient.r >= 0)
		return (VEC3_INF);

	int		width = textureSize(environment_map, 0).x;
	int		height = textureSize(environment_map, 0).y;
	float	random_value;
	vec3	direction;
	ivec2	pixel_pos;
	vec2	uv;

	pixel_pos = sample_environment_cdf(width, height);
	pdf = texelFetch(environment_map, ivec3(pixel_pos, 1), 0).g;

	// random offset to prevent 'stadium lights effect'
	uv = (vec2(pixel_pos) + 0.5 + sample_uniform_disc(1.0)) / vec2(width, height);

	float theta = (uv.x - 0.5) * (2.0 * M_PI);
	float phi = (0.5 - uv.y) * (M_PI);
	float cos_phi = cos(phi);
	direction.x = cos_phi * cos(theta);
	direction.y = -cos_phi * sin(theta);
	direction.z = -sin(phi);

	return (direction);
}

vec3 sample_hemisphere(vec3 N)
{
	vec2 r = vec2(rand(), rand());

	float theta = acos(sqrt(r.x));
	float phi = 2.0 * M_PI * r.y;

	vec3 bounce;
	bounce.x = sin(theta) * cos(phi);
	bounce.y = sin(theta) * sin(phi);
	bounce.z = cos(theta);

	vec3 tangent = normalize(abs(N.y) == 1.0 ? cross(vec3(1, 0, 0), N) : cross(vec3(0, 1, 0), N));
	vec3 bitangent = cross(N, tangent);
	bounce = bounce.x * tangent + bounce.y * bitangent + bounce.z * N;

	return (normalize(bounce));
}

vec2 sample_uniform_disc(float diameter)
{
	if (diameter == 0)
		return (vec2(0, 0));
	float angle = rand() * M_PI * 2;
	vec2  point = vec2(cos(angle), sin(angle));
	point *= sqrt(rand()) * (diameter / 2);

	return (point);
}

vec3 sample_visible_normal(vec3 V, vec3 N, float roughness)
{
	// Transform view vector to tangent space
	vec3 T1 = normalize(abs(N.z) < 0.999 ? cross(vec3(0.0, 0.0, 1.0), N) : vec3(1.0, 0.0, 0.0));
	vec3 T2 = cross(N, T1);
	vec3 Vh = normalize(mat3(T1, T2, N) * V);

	// Generate random numbers
	vec2 r = vec2(rand(), rand());

	// Sample the GGX VNDF
	float a = roughness * roughness;
	float phi = 2.0 * M_PI * r.x;
	float cosTheta = sqrt((1.0 - r.y) / (1.0 + (a * a - 1.0) * r.y));
	float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

	vec3 H = vec3(sinTheta * cos(phi), sinTheta * sin(phi), cosTheta);
	
	// Transform H back to world space
	H = normalize(mat3(T1, T2, N) * H);

	return (H);
}