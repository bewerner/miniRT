vec3	solid_illuminate_from_point_light(t_point_light point_light, t_hitpoint hitpoint)
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
	if (intensity <= 0.000005)
		return (VEC3_BLACK);
	intensity *= point_light.intensity;
	return (intensity * point_light.color);
}


vec3	get_solid_specular_color(t_point_light light, float roughness, t_hitpoint hitpoint)
{
	vec3	col;
	float	intensity;
	vec3	bounce_dir;
	vec3	light_dir;

	col = VEC3_BLACK;

	bounce_dir = normalize(mirror(hitpoint.ray, hitpoint.normal));
	light_dir = normalize(light.origin - hitpoint.pos);

	intensity = dot(bounce_dir, light_dir);
	if (intensity > 0)
	{
		intensity = 1 * pow(intensity, roughness);
		col += intensity * solid_illuminate_from_point_light(light, hitpoint);
	}

	return (col);
}
