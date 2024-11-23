vec3	get_environment_map_color(vec3 direction)
{
	vec2	uv;

	vec3 normal = normalize(direction);
	uv.x = 0.5 + atan(-normal.y, normal.x) / (2.0 * M_PI);
	uv.y = 0.5 - asin(-normal.z) / M_PI;

	return (texture(environment_map, vec3(uv, 0)).rgb);
}

vec3	get_ambient_color(vec3 dir)
{
	if (rt.ambient.r >= 0)
		return (rt.ambient);
	return (get_environment_map_color(dir));
}
