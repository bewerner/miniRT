vec3	get_checker_color(int tex_idx, t_hitpoint hitpoint)
{
	ivec3	pos;
	// if (rt.debug == 0)
	// 	pos = ivec3(abs(floor(hitpoint.pos / textures[tex_idx].scale)));
	// else
		pos = ivec3(abs(floor((hitpoint.pos + hitpoint.normal * 0.0000005 * length(hitpoint.ray)) / textures[tex_idx].scale)));

	if ((pos.x + pos.y + pos.z) % 2 == 1)
		return (textures[tex_idx].col2);

	return (textures[tex_idx].col1);
}
