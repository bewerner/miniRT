vec4	get_diffuse_color(t_hitpoint hitpoint)
{
	// return (materials[hitpoint.material_idx].color);
	return (hitpoint.color);
}
