vec3	get_checker_color(int tex_idx, t_hitpoint hitpoint)
{
	// float	scale = 0.2;
	// vec3	col_a = VEC3_BLACK;
	// vec3	col_b = VEC3_WHITE;
	float	scale = textures[tex_idx].scale;
	vec3	col_a = textures[tex_idx].col1;
	vec3	col_b = textures[tex_idx].col2;
	vec3	col = col_a;
	ivec3	pos = ivec3(abs(floor(get_offset_hitpoint_pos(hitpoint) / scale)));

	if ((pos.x + pos.y + pos.z) % 2 == 1)
		col = col_b;
	return (col);
}
