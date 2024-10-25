vec3	get_normal_color(t_hitpoint hitpoint)
{
	vec3	final_col;
	vec3	light_col;

	final_col = VEC3_BLACK;

	// int i = -1;
	// int type = next_light_type(i);
	final_col = hitpoint.normal;
	return (final_col);
}
