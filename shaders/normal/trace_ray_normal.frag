vec3	trace_ray_normal(t_ray ray)
{
	t_hitpoint	hitpoint;
	vec3		col;

	hitpoint = get_closest_hitpoint(ray, true);
	if (!hitpoint.hit)
		return (VEC3_BLACK);

	col = hitpoint.normal * 0.5 + 0.5;

	return (col);
}
