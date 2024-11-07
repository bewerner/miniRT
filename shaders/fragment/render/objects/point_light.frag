t_point_light	get_point_light(int offset)
{
	t_point_light	point_light;

	point_light.type = int(texelFetch(lights, offset++).r);
	point_light.next_offset = int(texelFetch(lights, offset++).r);
	point_light.origin = vec3(texelFetch(lights, offset++).r, texelFetch(lights, offset++).r, texelFetch(lights, offset++).r);
	point_light.color = vec3(texelFetch(lights, offset++).r, texelFetch(lights, offset++).r, texelFetch(lights, offset++).r);
	point_light.power = texelFetch(lights, offset++).r;
	point_light.intensity = texelFetch(lights, offset++).r;
	point_light.radius = texelFetch(lights, offset++).r;

	if (rt.mode == MODE_RENDER && point_light.radius > 0.0)
	{
		float	z = mix(-1.0, 1.0, rand());
		float	radial_distance = sqrt(1 - z * z);
		float	phi = mix(0.0, 2 * M_PI, rand());
		float	x = radial_distance * cos(phi);
		float	y = radial_distance * sin(phi);
		vec3	offset_vec = vec3(x, y, z);

		point_light.origin += offset_vec * point_light.radius;
	}

	return (point_light);
}
