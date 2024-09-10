vec3	illuminate_from_point_light(t_point_light point_light, t_hitpoint hitpoint)
{
	t_ray	light_ray;
	float	distance;
	float	intensity;

	light_ray.dir = get_offset_hitpoint_pos(hitpoint) - point_light.origin;
	light_ray.origin = point_light.origin;
	intensity = -dot(hitpoint.normal, normalize(light_ray.dir));
	if (intensity <= 0)
		return (VEC3_BLACK);
	distance = length(light_ray.dir);
	intensity = intensity / (distance * distance);
	if (intensity <= 0.000005 || is_obstructed(light_ray))
		return (VEC3_BLACK);
	intensity *= point_light.intensity;
	return (intensity * point_light.color);
}

vec3	get_illumination_color(t_hitpoint hitpoint)
{
	vec3 col = VEC3_BLACK;

	int i = -1;
	int type = next_light_type(i);
	while (type != LIGHT_NONE)
	{
		col += illuminate_from_point_light(get_point_light(i), hitpoint);
		type = next_light_type(i);
	}
	return (col);
}
