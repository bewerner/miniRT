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
	if (intensity <= 0.000005 || is_obstructed(light_ray))
		return (VEC4_BLACK);
	intensity *= point_light.intensity;
	return (intensity * point_light.color);
}

int	next_light_type(inout int i)
{
	if (i == -1)
	{
		i++;
		return (int(texelFetch(lights, i).r));
	}
	i = int(texelFetch(lights, i + 1).r);
	return (int(texelFetch(lights, i).r));
}

vec4	get_normal_color(t_hitpoint hitpoint)
{
	vec4	final_col;
	vec4	light_col;

	final_col = VEC4_BLACK;

	int i = -1;
	int type = next_light_type(i);
	final_col = vec4(hitpoint.normal, 1);
	return (final_col);
}
