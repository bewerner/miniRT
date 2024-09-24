vec3	get_checker_color(int tex_idx, t_hitpoint hitpoint)
{
	// float	scale = textures[tex_idx].scale;
	// vec3		col_a = textures[tex_idx].col1;
	// vec3		col_b = textures[tex_idx].col2;

	// ivec3	pos = ivec3(abs(floor(get_offset_hitpoint_pos(hitpoint) / scale)));
	ivec3	pos = ivec3(abs(floor(hitpoint.pos / textures[tex_idx].scale)));

	if ((pos.x + pos.y + pos.z) % 2 == 1)
		return (textures[tex_idx].col2);

	return (textures[tex_idx].col1);
}
