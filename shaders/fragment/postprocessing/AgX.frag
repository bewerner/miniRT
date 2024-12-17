vec3 to_agx(vec3 col)
{
	if (rt.view_transform == VIEW_AGX)
	{
		vec3 pos = (255.0 / 11.513 * log(100000.0 * (col / 255.0) + 1.0)) / 255.0;
		col = texture(agx_lut, pos).rgb;
	}
	col = pow(col, vec3(1 / 2.2));
	return (col);
}
