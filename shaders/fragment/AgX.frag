vec3 look_up(vec3 exact_pos)
{
	ivec3 pos = ivec3(exact_pos);

	vec3 col_0_0_0 = texelFetch(agx_lut, (pos.x + 0) + ((pos.y + 0) * 32) + ((pos.z + 0) * 32 * 32)).rgb;
	vec3 col_1_0_0 = texelFetch(agx_lut, (pos.x + 1) + ((pos.y + 0) * 32) + ((pos.z + 0) * 32 * 32)).rgb;
	vec3 col_0_1_0 = texelFetch(agx_lut, (pos.x + 0) + ((pos.y + 1) * 32) + ((pos.z + 0) * 32 * 32)).rgb;
	vec3 col_1_1_0 = texelFetch(agx_lut, (pos.x + 1) + ((pos.y + 1) * 32) + ((pos.z + 0) * 32 * 32)).rgb;

	vec3 col_0_0_1 = texelFetch(agx_lut, (pos.x + 0) + ((pos.y + 0) * 32) + ((pos.z + 1) * 32 * 32)).rgb;
	vec3 col_1_0_1 = texelFetch(agx_lut, (pos.x + 1) + ((pos.y + 0) * 32) + ((pos.z + 1) * 32 * 32)).rgb;
	vec3 col_0_1_1 = texelFetch(agx_lut, (pos.x + 0) + ((pos.y + 1) * 32) + ((pos.z + 1) * 32 * 32)).rgb;
	vec3 col_1_1_1 = texelFetch(agx_lut, (pos.x + 1) + ((pos.y + 1) * 32) + ((pos.z + 1) * 32 * 32)).rgb;

	vec3 factor = fract(exact_pos);

	vec3 col = mix
	(
		mix
		(
			mix(col_0_0_0, col_1_0_0, factor.x),
			mix(col_0_1_0, col_1_1_0, factor.x),
			factor.y
		),
		mix
		(
			mix(col_0_0_1, col_1_0_1, factor.x),
			mix(col_0_1_1, col_1_1_1, factor.x),
			factor.y
		),
		factor.z
	);

	return (col);
}

vec3 to_agx(vec3 col)
{
	vec3	pos;

	col = clamp(col, 0.0, 255.0);
	pos = (255.0 / 11.61 * log(100000.0 * (col / 277.438) + 1.0)) / 255.0 * 32.0 - 0.5;
	pos = clamp(pos, 0.0, 30.999);
	col = look_up(pos);
	col = pow(col, vec3(1 / 2.2));
	col -= pow(1 - col, vec3(5)) * 0.04;
	return (col);
}
