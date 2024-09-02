
vec4	get_normal_color(t_hitpoint hitpoint)
{
	vec4	final_col;
	vec4	light_col;

	final_col = VEC4_BLACK;

	// int i = -1;
	// int type = next_light_type(i);
	final_col = vec4(hitpoint.normal, 1);
	return (final_col);
}
