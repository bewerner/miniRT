vec3	apply_normal_map(t_hitpoint hitpoint)
{
	float	strength = materials[hitpoint.material_idx].normal_strength;
	vec3	tex_normal = get_color_from_texture(materials[hitpoint.material_idx].normal_map_idx, hitpoint);
	// tex_normal = pow(tex_normal, vec3(rt.debug2/10));

	// if (rt.debug2 == 1)
	// 	tex_normal = pow(tex_normal, vec3(1 / 2.2));
	// if (rt.debug2 == 2)
	// 	tex_normal = pow(tex_normal, vec3(2.2));
	tex_normal = normalize(tex_normal * 2 - vec3(1));
	tex_normal = mix(vec3(0, 0, 1), tex_normal, strength);
	if (strength > 1)
		tex_normal.z = max(tex_normal.z, 1);
	tex_normal.z = clamp(tex_normal.z, 0, 1);
	tex_normal = normalize(tex_normal);

	return	(tex_normal.x * hitpoint.tangent +
			 tex_normal.y * hitpoint.bitangent + 
			 tex_normal.z * hitpoint.normal);
}
