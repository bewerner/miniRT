vec4	get_specular_color(t_hitpoint hitpoint, vec4 col_illumination)
{
	vec4	col;
	float	intensity;
	vec3	bounce_dir;
	vec3	light_dir;

	col = VEC4_BLACK;
	int i = -1;
	int type = next_light_type(i);
	while (type != LIGHT_NONE)
	{
		if (type == LIGHT_POINT)
		{
			t_point_light light = get_point_light(i);
			bounce_dir = normalize(create_bounce_dir(hitpoint.ray, hitpoint.normal));
			light_dir = normalize(light.origin - hitpoint.pos);
			intensity = dot(bounce_dir, light_dir);
			if (intensity > 0)
			{
				intensity = 1 * pow(intensity, 10); // multiply with specular value (0.0 - 1.0) and to the power of some roughness calculation
				col += intensity * col_illumination;
			}
		}
		type = next_light_type(i);
	}
	return (col * hitpoint.color * 1.0);
}
