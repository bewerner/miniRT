vec3	get_color_from_texture(int tex_idx, t_hitpoint hitpoint)
{
	// tex_idx = 1;
	if (textures[tex_idx].type == TEX_IMAGE)
		return (vec3(0.5, 0, 1));
	else if (textures[tex_idx].type == TEX_CHECKER)
		return(get_checker_color(tex_idx, hitpoint));
	
	// INTERMEDIATE DEFAULT ERROR COLOR
	return (vec3(1, 0, 0.5));
}

vec3	get_diffuse_color(t_hitpoint hitpoint)
{
	if (hitpoint.material_idx > 0 && materials[hitpoint.material_idx].color_tex_idx >= 0)
		return (get_color_from_texture(materials[hitpoint.material_idx].color_tex_idx, hitpoint));
	// return(get_checker_color(hitpoint));
	// return (materials[hitpoint.material_idx].color);
	return (hitpoint.color);
}
