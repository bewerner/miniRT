t_point_light	get_point_light(int offset)
{
	t_point_light	point_light;

	point_light.type = int(texelFetch(lights, offset++).r);
	point_light.next_offset = int(texelFetch(lights, offset++).r);
	point_light.origin = vec3(texelFetch(lights, offset++).r, texelFetch(lights, offset++).r, texelFetch(lights, offset++).r);
	point_light.color = vec3(texelFetch(lights, offset++).r, texelFetch(lights, offset++).r, texelFetch(lights, offset++).r);
	point_light.power = texelFetch(lights, offset++).r;
	point_light.intensity = texelFetch(lights, offset++).r;

	return (point_light);
}
