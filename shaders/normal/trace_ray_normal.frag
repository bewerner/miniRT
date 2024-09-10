vec3	trace_ray_normal(t_ray ray)
{
	t_hitpoint	hitpoint;
	vec3		col;

	hitpoint = get_closest_hitpoint(ray);
	if (!hitpoint.hit)
		return (VEC3_BLACK);

	// col = vec3(hitpoint.normal.r * 0.5 + 0.5, hitpoint.normal.g * 0.5 + 0.5, hitpoint.normal.b * 0.5 + 0.5, 1.0);
	col = hitpoint.normal * 0.5 + 0.5;

	return (col);
}
