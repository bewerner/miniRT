float	get_bump_height(int bump_idx, vec2 uv, t_hitpoint hitpoint)
{
	if (textures[bump_idx].type == TEX_IMAGE)
	{
		switch (textures[bump_idx].texture_unit) {
			case 0:
				return ( (texture(texture_units[0], uv).r + texture(texture_units[0], uv).g + texture(texture_units[0], uv).b) / 3 );
			case 1:
				return ( (texture(texture_units[1], uv).r + texture(texture_units[1], uv).g + texture(texture_units[1], uv).b) / 3 );
			case 2:
				return ( (texture(texture_units[2], uv).r + texture(texture_units[2], uv).g + texture(texture_units[2], uv).b) / 3 );
			case 3:
				return ( (texture(texture_units[3], uv).r + texture(texture_units[3], uv).g + texture(texture_units[3], uv).b) / 3 );
			case 4:
				return ( (texture(texture_units[4], uv).r + texture(texture_units[4], uv).g + texture(texture_units[4], uv).b) / 3 );
			case 5:
				return ( (texture(texture_units[5], uv).r + texture(texture_units[5], uv).g + texture(texture_units[5], uv).b) / 3 );
			case 6:
				return ( (texture(texture_units[6], uv).r + texture(texture_units[6], uv).g + texture(texture_units[6], uv).b) / 3 );
			case 7:
				return ( (texture(texture_units[7], uv).r + texture(texture_units[7], uv).g + texture(texture_units[7], uv).b) / 3 );
			case 8:
				return ( (texture(texture_units[8], uv).r + texture(texture_units[8], uv).g + texture(texture_units[8], uv).b) / 3 );
			case 9:
				return ( (texture(texture_units[9], uv).r + texture(texture_units[9], uv).g + texture(texture_units[9], uv).b) / 3 );
			default:
				return (-1.0);
		}
	}		
	else if (textures[bump_idx].type == TEX_CHECKER)
	{
		vec3 bump_col = get_checker_color(bump_idx, hitpoint);
		return( (bump_col.r + bump_col.g + bump_col.b) / 3 );
	}
	else
		return (-1.0);
}

vec3	apply_bump_map(t_hitpoint hitpoint)
{
	int	bump_idx = materials[hitpoint.material_idx].bump_map_idx;

	if (bump_idx < 0)
		return hitpoint.normal;

	// Calculate du and dv based on the texture resolution
	float	du = 1.0 / 2048 / 10;
	float	dv = 1.0 / 2048 / 10;

	// Get the height at the current texture coordinate
	float	H = get_bump_height(bump_idx, hitpoint.uv, hitpoint);

	float	Hup = H - get_bump_height(bump_idx, hitpoint.uv + vec2(du, 0.0), hitpoint);
	float	Hum = H - get_bump_height(bump_idx, hitpoint.uv - vec2(du, 0.0), hitpoint);
	float	Hvp = H - get_bump_height(bump_idx, hitpoint.uv + vec2(0.0, dv), hitpoint);
	float	Hvm = H - get_bump_height(bump_idx, hitpoint.uv - vec2(0.0, dv), hitpoint);

	// float	bump_strength = rt.debug / 50;
	// vec3	bump_normal = H * hitpoint.normal + bump_strength * normalize((Hum * hitpoint.tangent - Hup * hitpoint.tangent) + (Hvm * hitpoint.bitangent - Hvp * hitpoint.bitangent));
	vec3	bump_normal = mix(	hitpoint.normal,
								hitpoint.normal * H + normalize((Hum * hitpoint.tangent - Hup * hitpoint.tangent) + (Hvm * hitpoint.bitangent - Hvp * hitpoint.bitangent)),
								materials[hitpoint.material_idx].bump_strength);

	// Re-normalize the bump_normal
	return ( normalize(bump_normal) );
}

// vec3	apply_bump_map(t_hitpoint hitpoint)
// {
// 	int	bump_idx = materials[hitpoint.material_idx].bump_map_idx;


// 	if (bump_idx < 0) // || textures[materials[hitpoint.material_idx].bump_map_idx].texture_unit < 0
// 		return hitpoint.normal;

// 	// Calculate du and dv based on the texture resolution
// 	float	du = 1.0 / 2048 / 10;
// 	float	dv = 1.0 / 2048 / 10;

// 	// Get the height at the current texture coordinate
// 	// float H = texture(textures[materials[hitpoint.material_idx].bump_map_idx].texture_unit, hitpoint.uv).r;
// 	float	H = get_bump_height(bump_idx, hitpoint.uv, hitpoint);

// 	// Get heights at neighboring texels using central differences
// 	// float	H_u = texture(textures[materials[hitpoint.material_idx].bump_map_idx].texture_unit, hitpoint.uv + vec2(du, 0.0)).r - texture(textures[materials[hitpoint.material_idx].bump_map_idx].texture_unit, hitpoint.uv - vec2(du, 0.0)).r;
// // float	H_u = get_bump_height(bump_idx, hitpoint.uv + vec2(du, 0.0), hitpoint) - get_bump_height(bump_idx, hitpoint.uv - vec2(du, 0.0), hitpoint);
// 	// float	H_v = texture(textures[materials[hitpoint.material_idx].bump_map_idx].texture_unit, hitpoint.uv + vec2(0.0, dv)).r - texture(textures[materials[hitpoint.material_idx].bump_map_idx].texture_unit, hitpoint.uv - vec2(0.0, dv)).r;
// // float	H_v = get_bump_height(bump_idx, hitpoint.uv + vec2(0.0, dv), hitpoint) - get_bump_height(bump_idx, hitpoint.uv - vec2(0.0, dv), hitpoint);

// 	float	Hup = H - get_bump_height(bump_idx, hitpoint.uv + vec2(du, 0.0), hitpoint);
// 	float	Hum = H - get_bump_height(bump_idx, hitpoint.uv - vec2(du, 0.0), hitpoint);
// 	float	Hvp = H - get_bump_height(bump_idx, hitpoint.uv + vec2(0.0, dv), hitpoint);
// 	float	Hvm = H - get_bump_height(bump_idx, hitpoint.uv - vec2(0.0, dv), hitpoint);

// 	float	bump_strength = rt.debug / 10;

// 	vec3	bump_normal = H * hitpoint.normal + bump_strength * normalize((Hum * hitpoint.tangent - Hup * hitpoint.tangent) + (Hvm * hitpoint.bitangent - Hvp * hitpoint.bitangent));
// 	// vec3	bump_normal = mix(hitpoint.normal, hitpoint.normal * H + normalize((Hum * hitpoint.tangent - Hup * hitpoint.tangent) + (Hvm * hitpoint.bitangent - Hvp * hitpoint.bitangent)), rt.debug/10);

// 	// calc the normal using the slopes in the bump map
// 	// vec3	bump_normal = H * hitpoint.normal + H_u * hitpoint.tangent + H_v * hitpoint.bitangent;

// 	// return ( hitpoint.bitangent);

// 	// Re-normalize the bump_normal
// 	return ( normalize(bump_normal) );
// }
