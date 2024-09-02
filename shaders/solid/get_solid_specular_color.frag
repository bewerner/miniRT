vec4	illuminate_from_point_light(t_point_light point_light, t_hitpoint hitpoint)
{
	t_ray	light_ray;
	float	distance;
	float	intensity;

	light_ray.dir = (hitpoint.pos + 0.001 * hitpoint.normal) - point_light.origin;
	light_ray.origin = point_light.origin;

	intensity = -dot(hitpoint.normal, normalize(light_ray.dir));
	if (intensity <= 0)
		return (VEC4_BLACK);

	distance = length(light_ray.dir);

	intensity = intensity / (distance * distance);
	if (intensity <= 0.000005)
		return (VEC4_BLACK);

	intensity *= point_light.intensity;
	return (intensity * point_light.color);
}

vec4	get_solid_specular_color(t_point_light light, float roughness, t_hitpoint hitpoint)
{
	vec4	col;
	float	intensity;
	vec3	bounce_dir;
	vec3	light_dir;

	col = VEC4_BLACK;

	bounce_dir = normalize(create_bounce_dir(hitpoint.ray, hitpoint.normal));
	light_dir = normalize(light.origin - hitpoint.pos);

	intensity = dot(bounce_dir, light_dir);
	if (intensity > 0)
	{
		intensity = 1 * pow(intensity, roughness); // multiply with specular value (0.0 - 1.0) and to the power of some roughness calculation
		col += intensity * illuminate_from_point_light(light, hitpoint);
	}

	return (col);
}
