vec4	shoot_ray(t_ray ray)
{
	t_hitpoint	hitpoint;
	vec4		col_diff;

	hitpoint = get_closest_hitpoint(ray);
	if (!hitpoint.hit)
		return VEC4_BLACK;

	col_diff = get_diffuse_color(hitpoint);
	return (col_diff);
}


vec4	get_reflection_color(t_hitpoint hp)
{
	vec4	col;
	t_ray	bounce_ray;

	bounce_ray.origin = hp.pos + 0.001f * hp.normal;
	bounce_ray.dir = create_bounce_dir(hp.ray, hp.normal);
	col = shoot_ray(bounce_ray);
		
	// include the base_color as meteallic materials do it like this is it mul, or add???

	return (col);
}