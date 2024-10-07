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

ivec2	get_texture_resolution(int tex_idx)
{
	if (textures[tex_idx].type == TEX_IMAGE)
	{
		switch (textures[tex_idx].texture_unit) {
			case 0:
				return ( textureSize(texture_units[0], 0) );
			case 1:
				return ( textureSize(texture_units[1], 0) );
			case 2:
				return ( textureSize(texture_units[2], 0) );
			case 3:
				return ( textureSize(texture_units[3], 0) );
			case 4:
				return ( textureSize(texture_units[4], 0) );
			case 5:
				return ( textureSize(texture_units[5], 0) );
			case 6:
				return ( textureSize(texture_units[6], 0) );
			case 7:
				return ( textureSize(texture_units[7], 0) );
			case 8:
				return ( textureSize(texture_units[8], 0) );
			case 9:
				return ( textureSize(texture_units[9], 0) );
			default:
				return (ivec2(-1, -1));
		}
	}		
	return (ivec2(-1, -1));
}

vec3	get_hitpoint_color(t_hitpoint hitpoint)
{
	if (hitpoint.color.r >= 0)
		return (hitpoint.color);
	else if (materials[hitpoint.material_idx].color.r >= 0)
		return (materials[hitpoint.material_idx].color);
	return (get_color_from_texture(materials[hitpoint.material_idx].color_tex_idx, hitpoint));
}

vec3	get_offset_hitpoint_pos(t_hitpoint hitpoint)
{
	return (hitpoint.pos + (max(1, length(hitpoint.ray)) * 0.0001) * hitpoint.object_normal);
}

t_hitpoint	get_closest_hitpoint(t_ray ray, bool init_all)
{
	t_hitpoint	closest;
	t_hitpoint	current;

	closest = HP_INF;
	current = HP_INF;

	int i = -1;
	int type = next_object_type(i);
	while (type != OBJ_NONE)
	{
		if (type == OBJ_SPHERE)
			current = get_hitpoint_sphere(ray, get_sphere(i), init_all);
		else if (type == OBJ_PLANE)
			current = get_hitpoint_plane(ray, get_plane(i), init_all);
		else if (type == OBJ_CYLINDER)
			current = get_hitpoint_cylinder(ray, get_cylinder(i), init_all);
		else if (type == OBJ_HYPERBOLOID)
			current = get_hitpoint_hyperboloid(ray, get_hyperboloid(i), init_all);
		if (length(current.ray) < length(closest.ray))
			closest = current;
		type = next_object_type(i);
	}
	return (closest);
}

// The following two functions could be replaced with a flag in the material
// bool	has_img_tex;
// for example, which could be set in the parsing, and save valuable time
// in the rendering process. This could also be expandet to and int,
// which is a flag for for CALC_UV and CALC_normal_MAP and maybe even more…
bool	has_image_texture(t_hitpoint hitpoint, out bool texture_is_square)
{
	if (hitpoint.material_idx <= 0)
		return (false);

	t_material material = materials[hitpoint.material_idx]; // instantiating a t_material here actually increases performance a lot
	if (material.color_tex_idx >= 0)
	{
		ivec2 resolution = get_texture_resolution(material.color_tex_idx);
		texture_is_square = (resolution.x == resolution.y);
		return (true);
	}
	if (material.normal_map_idx >= 0)
	{
		ivec2 resolution = get_texture_resolution(material.normal_map_idx);
		texture_is_square = (resolution.x == resolution.y);
		return (true);
	}
	return (false);
}

bool	has_image_texture(t_hitpoint hitpoint)
{
	if (hitpoint.material_idx <= 0)
		return (false);

	t_material material = materials[hitpoint.material_idx]; // instantiating a t_material here actually increases performance a lot
	if (material.color_tex_idx >= 0)
		return (true);
	if (material.normal_map_idx >= 0)
		return (true);
	return (false);
}

bool	has_normal_map_material(t_hitpoint hitpoint)
{
	if (hitpoint.material_idx <= 0)
		return (false);

	if (materials[hitpoint.material_idx].normal_map_idx >= 0)
		return (true);
	return (false);
}
