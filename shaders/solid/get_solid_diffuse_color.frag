vec4	get_solid_color(t_hitpoint hitpoint)
{
	vec4	col;
	vec4	solid_ambient = vec4(0.25, 0.25, 0.25, 1);
	vec4	solid_light   = vec4(0.55, 0.55, 0.55, 1);
	float	my_dot;

	if (hitpoint.hit == false)
		return (rt.ambient);

	my_dot = dot(rt.camera.viewport_light, hitpoint.normal);
	my_dot = max(my_dot, 0);
	col = my_dot * solid_light;
	col = col + solid_ambient;
	return (col);
}
