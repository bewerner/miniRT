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
	if (materials[hitpoint.material_idx].color.r >= 0)
		return (materials[hitpoint.material_idx].color);
	// return (get_color_from_texture(materials[hitpoint.material_idx].color_tex_idx, hitpoint));
	vec3 col = get_color_from_texture(materials[hitpoint.material_idx].color_tex_idx, hitpoint);
	return (pow(col, vec3(2.2)));
}

float	get_hitpoint_metallic(t_hitpoint hitpoint)
{
	if (materials[hitpoint.material_idx].metallic >= 0.0)
		return (materials[hitpoint.material_idx].metallic);
	vec3 col = get_color_from_texture(materials[hitpoint.material_idx].metallic_tex_idx, hitpoint);
	// if (textures[materials[hitpoint.material_idx].metallic_tex_idx].type == TEX_IMAGE)
	// 	col = pow(col, vec3(1 / 2.2));
	return ((col.r + col.g + col.b) / 3);
}

float	get_hitpoint_roughness(t_hitpoint hitpoint)
{
	if (materials[hitpoint.material_idx].roughness >= 0.0)
		return (materials[hitpoint.material_idx].roughness);
	vec3 col = get_color_from_texture(materials[hitpoint.material_idx].roughness_tex_idx, hitpoint);
	// if (textures[materials[hitpoint.material_idx].roughness_tex_idx].type == TEX_IMAGE)
	// 	col = pow(col, vec3(1 / 2.2));
	return ((col.r + col.g + col.b) / 3);
}

vec3	get_offset_hitpoint_pos(t_hitpoint hitpoint)
{
	return (hitpoint.pos + (max(10, length(hitpoint.ray)) * 0.0001) * hitpoint.object_normal);
}

t_hitpoint	get_hitpoint(t_ray ray, int object_idx, bool init_all)
{
	if (object_idx < 0)
		return (HP_INF);

	t_hitpoint	hitpoint;

	int type = int(texelFetch(objects, object_idx).r);
	if (type == OBJ_SPHERE)
		hitpoint = get_hitpoint_sphere(ray, get_sphere(object_idx), init_all);
	else if (type == OBJ_PLANE)
		hitpoint = get_hitpoint_plane(ray, get_plane(object_idx), init_all);
	else if (type == OBJ_CYLINDER)
		hitpoint = get_hitpoint_cylinder(ray, get_cylinder(object_idx), init_all);
	else if (type == OBJ_HYPERBOLOID)
		hitpoint = get_hitpoint_hyperboloid(ray, get_hyperboloid(object_idx), init_all);
	hitpoint.object_idx = object_idx;

	return (hitpoint);
}

t_hitpoint	get_closest_hitpoint(t_ray ray, bool init_all)
{
	t_hitpoint	closest;
	t_hitpoint	current;

	closest = HP_INF;
	current = HP_INF;

	int object_idx = 0;
	while (object_idx >= 0)
	{
		current = get_hitpoint(ray, object_idx, false);
		if (length(current.ray) < length(closest.ray))
			closest = current;
		object_idx = next_object_index(object_idx);
	}
	if (init_all == true && closest.hit == true)
		return (get_hitpoint(ray, closest.object_idx, true));
	return (closest);
}

t_hitpoint	get_previous_hitpoint(vec2 uv)
{
	int object_idx = int(texture(buffer, vec3(uv, 2.0)).a);
	if (object_idx < 0)
		return (HP_INF);

	t_ray		ray;
	t_hitpoint	hitpoint;

	hitpoint.object_idx = object_idx;

	ray.origin = texture(buffer, vec3(uv, 3.0)).rgb;
	ray.dir    = texture(buffer, vec3(uv, 2.0)).rgb;

	return (get_hitpoint(ray, object_idx, true));
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
	else if (material.normal_map_idx >= 0)
	{
		ivec2 resolution = get_texture_resolution(material.normal_map_idx);
		texture_is_square = (resolution.x == resolution.y);
		return (true);
	}
	else if (material.metallic_tex_idx >= 0)
	{
		ivec2 resolution = get_texture_resolution(material.metallic_tex_idx);
		texture_is_square = (resolution.x == resolution.y);
		return (true);
	}
	else if (material.roughness_tex_idx >= 0)
	{
		ivec2 resolution = get_texture_resolution(material.roughness_tex_idx);
		texture_is_square = (resolution.x == resolution.y);
		return (true);
	}
	return (false);
}

bool	has_image_texture(t_hitpoint hitpoint)
{
	if (hitpoint.material_idx <= 0)
		return (false);

	t_material material = materials[hitpoint.material_idx];
	if (material.color_tex_idx >= 0)
		return (true);
	else if (material.normal_map_idx >= 0)
		return (true);
	else if (material.metallic_tex_idx >= 0)
		return (true);
	else if (material.roughness_tex_idx >= 0)
		return (true);
	return (false);
}

bool	has_normal_map_material(t_hitpoint hitpoint)
{
	if (hitpoint.material_idx <= 0)
		return (false);

	t_material material = materials[hitpoint.material_idx];
	if (material.normal_map_idx >= 0)
		return (true);
	return (false);
}
