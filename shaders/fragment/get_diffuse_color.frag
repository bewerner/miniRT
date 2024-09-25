vec3	get_color_from_texture(int tex_idx, t_hitpoint hitpoint)
{
	if (textures[tex_idx].type == TEX_IMAGE)
	{
		switch (textures[tex_idx].texture_unit) {
			case 0:
				return ( texture(texture_units[0], hitpoint.uv).rgb );
			case 1:
				return ( texture(texture_units[1], hitpoint.uv).rgb );
			case 2:
				return ( texture(texture_units[2], hitpoint.uv).rgb );
			case 3:
				return ( texture(texture_units[3], hitpoint.uv).rgb );
			case 4:
				return ( texture(texture_units[4], hitpoint.uv).rgb );
			case 5:
				return ( texture(texture_units[5], hitpoint.uv).rgb );
			case 6:
				return ( texture(texture_units[6], hitpoint.uv).rgb );
			case 7:
				return ( texture(texture_units[7], hitpoint.uv).rgb );
			case 8:
				return ( texture(texture_units[8], hitpoint.uv).rgb );
			case 9:
				return ( texture(texture_units[9], hitpoint.uv).rgb );
			default:
				return (vec3(1, 0 ,0.5));
		}
	}		
	else if (textures[tex_idx].type == TEX_CHECKER)
		return(get_checker_color(tex_idx, hitpoint));
	
	// ELSE RETURN MISSING TEXTURE COLOR
	return (vec3(1, 0, 0.5));
}

vec3	get_diffuse_color(t_hitpoint hitpoint)
{
	if (hitpoint.color.r >= 0)
		return (hitpoint.color);
	else if (materials[hitpoint.material_idx].color.r >= 0)
		return (materials[hitpoint.material_idx].color);
	return (get_color_from_texture(materials[hitpoint.material_idx].color_tex_idx, hitpoint));
}
