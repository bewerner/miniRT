vec3	trace_ray_solid(t_ray ray, vec2 uv)
{
	t_hitpoint	hitpoint;
	vec3		col;
	vec3		col_diff = VEC3_BLACK;
	vec3		col_spec = VEC3_BLACK;
	vec3		solid_ambient = vec3(0.250980);

	int bounce_count = 0;

	hitpoint = get_closest_hitpoint(ray, false);
	if (!hitpoint.hit)
	{
		if (rt.mac_os == 1)
			return(vec3(0.266059 - length(uv) * 0.019));
		return (solid_ambient - vec3(length(uv) * 0.039216));
	}

	t_point_light light01;
	light01.origin = hitpoint.pos + rt.camera.viewport_light * 4.0;
	light01.color = vec3(0.8, 0.8, 0.8);
	light01.power = 8.0;
	light01.intensity = 2.5;

	t_point_light light02;
	light02.origin = hitpoint.pos + rt.camera.viewport_light * -8.0;
	light02.color = vec3(0.8, 0.7, 0.6);
	light02.power = 500.0;
	light02.intensity = 50;

	col_diff = get_solid_color(hitpoint);
	col_spec = get_solid_specular_color(light01, 20.0, hitpoint);
	col_spec += get_solid_specular_color(light02, 1.1, hitpoint) * 0.5;


	col = col_diff + col_spec;

	return (col);
}
