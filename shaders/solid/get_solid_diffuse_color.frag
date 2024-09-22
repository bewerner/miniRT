vec3	get_solid_color(t_hitpoint hitpoint)
{
	vec3	col;
	vec3	solid_ambient = vec3(0.215686);
	vec3	solid_light   = vec3(0.55, 0.55, 0.55);
	float	my_dot;

	if (rt.mac_os == 1)
		solid_ambient = vec3(0.247059);
	if (hitpoint.hit == false)
		return (rt.ambient);

	my_dot = dot(rt.camera.viewport_light, hitpoint.normal);
	my_dot = max(my_dot, 0);
	col = my_dot * solid_light;
	col = col + solid_ambient;
	return (col);
}
