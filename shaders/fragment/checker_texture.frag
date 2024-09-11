vec3	get_checker_color(t_hitpoint hitpoint)
{
	// float	scale = rt.debug / 10;
	float	scale = 0.2;
	vec3	col_a = VEC3_BLACK;
	vec3	col_b = VEC3_WHITE;
	vec3	col = col_a;
	ivec3	pos = ivec3(abs(floor(hitpoint.pos / scale)));

	if ((pos.x + pos.y + pos.z) % 2 == 1)
		col = col_b;
	return (col);
}
