vec3	get_diffuse_color(t_hitpoint hitpoint)
{
	// return(get_checker_color(hitpoint));
	// return (materials[hitpoint.material_idx].color);
	return (hitpoint.color);
}
