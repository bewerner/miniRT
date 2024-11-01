void	init_seed(void)
{
	g_seed = int(fract(sin(dot(vec2(coord.xy), vec2(12.9898, 78.233))) * 43758.5453123) * 5929 * (rt.sample_count + 1)) + rt.sample_count * 9823;
	g_seed += int(rand() * 943 * rt.sample_count);
	g_seed += int(rand() * 7943 * rt.diffuse_bounce_count);
	g_seed += int(rand() * 2348 * rt.glossy_bounce_count);
}

float	rand(void)
{
    g_seed = (214013 * g_seed + 2531011);
    return (float((g_seed >> 5) & 0x3FFFFFF) / 67108864.0);
}

vec2 random_point_in_circle(float diameter)
{
	if (diameter == 0)
		return (vec2(0, 0));
	float angle = rand() * M_PI * 2;
	vec2  point = vec2(cos(angle), sin(angle));
	point *= sqrt(rand()) * (diameter / 2);

	return (point);
}

vec3	dither(vec3 in_col)
{
	vec3 out_col;
	float	r = in_col.r * 255.0;
	float	g = in_col.g * 255.0;
	float	b = in_col.b * 255.0;

	if (rand() < r - floor(r))
		r++;
	if (rand() < g - floor(g))
		g++;
	if (rand() < b - floor(b))
		b++;

	r = floor(r);
	g = floor(g);
	b = floor(b);

	out_col.r = r / 255;
	out_col.g = g / 255;
	out_col.b = b / 255;

	return (out_col);
}

// vec3	dither(vec3 in_col)
// {
// 	vec3	out_col = in_col;
// 	float	r = in_col.r * 255.0;
// 	float	g = in_col.g * 255.0;
// 	float	b = in_col.b * 255.0;

// 	if (rand() < r - floor(r))
// 		out_col.r = in_col.r + 1.0 / 255.0;
// 	if (rand() < g - floor(g))
// 		out_col.g = in_col.g + 1.0 / 255.0;
// 	if (rand() < b - floor(b))
// 		out_col.b = in_col.b + 1.0 / 255.0;
// 	return (out_col);
// }
