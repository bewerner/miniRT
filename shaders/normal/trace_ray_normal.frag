vec4	trace_ray_normal(t_ray ray)
{
	t_hitpoint	hitpoint;
	vec4		col;

	hitpoint = get_closest_hitpoint(ray);
	if (!hitpoint.hit)
		return (vec4(0, 0, 0, 1));

	col = vec4(hitpoint.normal.r * 0.5 + 0.5, hitpoint.normal.g * 0.5 + 0.5, hitpoint.normal.b * 0.5 + 0.5, 1.0);

	return (col);
}
