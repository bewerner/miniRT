vec3	get_solid_color(t_hitpoint hitpoint)
{
	vec3	col;
	vec3	solid_ambient = vec3(0.25, 0.25, 0.25);
	vec3	solid_light   = vec3(0.55, 0.55, 0.55);
	float	my_dot;

	if (hitpoint.hit == false)
		return (rt.ambient);

	my_dot = dot(rt.camera.viewport_light, hitpoint.normal);
	my_dot = max(my_dot, 0);
	col = my_dot * solid_light;
	col = col + solid_ambient;
	return (col);
}
