vec3	get_normal_map_color(int normal_idx, vec2 uv, t_hitpoint hitpoint)
{
	if (textures[normal_idx].type == TEX_IMAGE)
	{
		switch (textures[normal_idx].texture_unit) {
			case 0:
				return (texture(texture_units[0], uv).rgb);
			case 1:
				return (texture(texture_units[1], uv).rgb);
			case 2:
				return (texture(texture_units[2], uv).rgb);
			case 3:
				return (texture(texture_units[3], uv).rgb);
			case 4:
				return (texture(texture_units[4], uv).rgb);
			case 5:
				return (texture(texture_units[5], uv).rgb);
			case 6:
				return (texture(texture_units[6], uv).rgb);
			case 7:
				return (texture(texture_units[7], uv).rgb);
			case 8:
				return (texture(texture_units[8], uv).rgb);
			case 9:
				return (texture(texture_units[9], uv).rgb);
			default:
				return (vec3(0, 0, 1));
		}
	}		
	else if (textures[normal_idx].type == TEX_CHECKER)
	{
		vec3 normal_col = get_checker_color(normal_idx, hitpoint);
		return( (normal_col.r + normal_col.g + normal_col.b) / 3 );
	}
	else
		return (vec3(0, 0, 1));
}

vec3	apply_normal_map(t_hitpoint hitpoint)
{
	int	normal_idx = materials[hitpoint.material_idx].normal_map_idx;

	if (normal_idx < 0)
		return hitpoint.normal;

	// Calculate du and dv based on the texture resolution
	float	du = 1.0 / 2048 / 1;
	float	dv = 1.0 / 2048 / 1;

	// Get the height at the current texture coordinate
	float	H = get_normal_map_color(normal_idx, hitpoint.uv, hitpoint);

	float	HUp = get_normal_map_color(normal_idx, hitpoint.uv + vec2(du, 0.0), hitpoint);
	float	HUm = get_normal_map_color(normal_idx, hitpoint.uv - vec2(du, 0.0), hitpoint);
	float	HVp = get_normal_map_color(normal_idx, hitpoint.uv + vec2(0.0, dv), hitpoint);
	float	HVm = get_normal_map_color(normal_idx, hitpoint.uv - vec2(0.0, dv), hitpoint);

	// float	normal_strength = rt.debug / 50;
	// vec3	normal_normal = H * hitpoint.normal + normal_strength * normalize((Hum * hitpoint.tangent - Hup * hitpoint.tangent) + (Hvm * hitpoint.bitangent - Hvp * hitpoint.bitangent));
	// vec3	normal_normal = mix(	hitpoint.normal,
	// 							hitpoint.normal * H + normalize((Hum * hitpoint.tangent - Hup * hitpoint.tangent) + (Hvm * hitpoint.bitangent - Hvp * hitpoint.bitangent)),
	// 							materials[hitpoint.material_idx].normal_strength);


	vec3	normal_normal = mix(	hitpoint.normal,
								hitpoint.normal * H + normalize((HUp - HUm) * hitpoint.tangent + (HVp - HVm) * hitpoint.bitangent),
								materials[hitpoint.material_idx].normal_strength	);

	// Re-normalize the normal_normal
	return ( normalize(normal_normal) );
}

// vec3	apply_normal_map(t_hitpoint hitpoint)
// {
// 	int	normal_idx = materials[hitpoint.material_idx].normal_map_idx;


// 	if (normal_idx < 0) // || textures[materials[hitpoint.material_idx].normal_map_idx].texture_unit < 0
// 		return hitpoint.normal;

// 	// Calculate du and dv based on the texture resolution
// 	float	du = 1.0 / 2048 / 10;
// 	float	dv = 1.0 / 2048 / 10;

// 	// Get the height at the current texture coordinate
// 	// float H = texture(textures[materials[hitpoint.material_idx].normal_map_idx].texture_unit, hitpoint.uv).r;
// 	float	H = get_normal_map_color(normal_idx, hitpoint.uv, hitpoint);

// 	// Get heights at neighboring texels using central differences
// 	// float	H_u = texture(textures[materials[hitpoint.material_idx].normal_map_idx].texture_unit, hitpoint.uv + vec2(du, 0.0)).r - texture(textures[materials[hitpoint.material_idx].normal_map_idx].texture_unit, hitpoint.uv - vec2(du, 0.0)).r;
// // float	H_u = get_normal_map_color(normal_idx, hitpoint.uv + vec2(du, 0.0), hitpoint) - get_normal_map_color(normal_idx, hitpoint.uv - vec2(du, 0.0), hitpoint);
// 	// float	H_v = texture(textures[materials[hitpoint.material_idx].normal_map_idx].texture_unit, hitpoint.uv + vec2(0.0, dv)).r - texture(textures[materials[hitpoint.material_idx].normal_map_idx].texture_unit, hitpoint.uv - vec2(0.0, dv)).r;
// // float	H_v = get_normal_map_color(normal_idx, hitpoint.uv + vec2(0.0, dv), hitpoint) - get_normal_map_color(normal_idx, hitpoint.uv - vec2(0.0, dv), hitpoint);

// 	float	Hup = H - get_normal_map_color(normal_idx, hitpoint.uv + vec2(du, 0.0), hitpoint);
// 	float	Hum = H - get_normal_map_color(normal_idx, hitpoint.uv - vec2(du, 0.0), hitpoint);
// 	float	Hvp = H - get_normal_map_color(normal_idx, hitpoint.uv + vec2(0.0, dv), hitpoint);
// 	float	Hvm = H - get_normal_map_color(normal_idx, hitpoint.uv - vec2(0.0, dv), hitpoint);

// 	float	normal_strength = rt.debug / 10;

// 	vec3	normal_normal = H * hitpoint.normal + normal_strength * normalize((Hum * hitpoint.tangent - Hup * hitpoint.tangent) + (Hvm * hitpoint.bitangent - Hvp * hitpoint.bitangent));
// 	// vec3	normal_normal = mix(hitpoint.normal, hitpoint.normal * H + normalize((Hum * hitpoint.tangent - Hup * hitpoint.tangent) + (Hvm * hitpoint.bitangent - Hvp * hitpoint.bitangent)), rt.debug/10);

// 	// calc the normal using the slopes in the normal map
// 	// vec3	normal_normal = H * hitpoint.normal + H_u * hitpoint.tangent + H_v * hitpoint.bitangent;

// 	// return ( hitpoint.bitangent);

// 	// Re-normalize the normal_normal
// 	return ( normalize(normal_normal) );
// }
