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

	if (point_light.radius > 0.0)
	{
		vec3	radius_offset = (vec3(rand(), rand(), rand()) * 2.0) - 1.0;
		point_light.origin += radius_offset * point_light.radius;
	}

	return (point_light);
}
