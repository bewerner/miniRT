vec4	get_solid_specular_color(t_point_light light, float roughness, t_hitpoint hitpoint)
{
	vec4	col;
	float	intensity;
	vec3	bounce_dir;
	vec3	light_dir;

	col = VEC4_BLACK;

	bounce_dir = normalize(create_bounce_dir(hitpoint.ray, hitpoint.normal));
	light_dir = normalize(light.origin - hitpoint.pos);

	intensity = dot(bounce_dir, light_dir);
	if (intensity > 0)
	{
		intensity = 1 * pow(intensity, roughness); // multiply with specular value (0.0 - 1.0) and to the power of some roughness calculation
		col += intensity * illuminate_from_point_light(light, hitpoint);
	}

	return (col);
}
