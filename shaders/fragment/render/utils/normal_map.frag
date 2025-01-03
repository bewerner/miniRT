vec3	apply_normal_map(t_hitpoint hitpoint)
{
	float	strength = materials[hitpoint.material_idx].normal_strength;
	vec3	tex_normal = get_texture_color(materials[hitpoint.material_idx].normal_map_idx, hitpoint);

	tex_normal = normalize(tex_normal * 2 - vec3(1));
	tex_normal = mix(vec3(0, 0, 1), tex_normal, strength);
	if (strength > 1)
		tex_normal.z = max(tex_normal.z, 1);
	tex_normal.z = clamp(tex_normal.z, 0, 1);
	tex_normal = normalize(tex_normal);

	vec3 normal =	(tex_normal.x * hitpoint.tangent +
					 tex_normal.y * hitpoint.bitangent + 
					 tex_normal.z * hitpoint.normal);
	return (normalize(normal));
}
