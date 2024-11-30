float rand_seeded(int seed)
{
	seed = (214013 * seed + 2531011);
	return (float((seed >> 5) & 0x3FFFFFF) / 67108864.0);
}
float easeInOutSine(float t) {
	return 0.5 * (1.0 - cos(t * M_PI));
}
vec3	get_noise_color(int tex_idx, t_hitpoint hitpoint)
{
	// float scale = rt.debug / 10;
	float scale = 1;
	float strength = rt.debug / 100;
	int	seed;
	vec2 pos = hitpoint.pos.xy * scale;
	vec2 c = floor(pos);
	vec2 b = c + vec2(1, 1);
	vec2 a = c + vec2(0, 1);
	vec2 d = c + vec2(1, 0);
	pos = fract(pos);

	vec3 col_a = vec3(0.0, 0.0, 1.0);
	vec3 col_b = vec3(0.0, 0.0, 1.0);
	vec3 col_c = vec3(0.0, 0.0, 1.0);
	vec3 col_d = vec3(0.0, 0.0, 1.0);

	seed = int(fract(sin(dot(a*0.82734, vec2(12.9898, 78.233))) * 43758.5453123) * 5929);
	col_a = vec3_rotate_x(col_a, rand_seeded(seed) * 0.5 * M_PI * strength);
	seed = int(fract(sin(dot(a*0.82734, vec2(32.9898, 68.233))) * 33758.5453123) * 2929);
	col_a = vec3_rotate_z(col_a, rand_seeded(seed) * 2 * M_PI * 999999);

	seed = int(fract(sin(dot(b*0.82734, vec2(12.9898, 78.233))) * 43758.5453123) * 5929);
	col_b = vec3_rotate_x(col_b, rand_seeded(seed) * 0.5 * M_PI * strength);
	seed = int(fract(sin(dot(b*0.82734, vec2(32.9898, 68.233))) * 33758.5453123) * 2929);
	col_b = vec3_rotate_z(col_b, rand_seeded(seed) * 2 * M_PI * 999999);

	seed = int(fract(sin(dot(c*0.82734, vec2(12.9898, 78.233))) * 43758.5453123) * 5929);
	col_c = vec3_rotate_x(col_c, rand_seeded(seed) * 0.5 * M_PI * strength);
	seed = int(fract(sin(dot(c*0.82734, vec2(32.9898, 68.233))) * 33758.5453123) * 2929);
	col_c = vec3_rotate_z(col_c, rand_seeded(seed) * 2 * M_PI * 999999);

	seed = int(fract(sin(dot(d*0.82734, vec2(12.9898, 78.233))) * 43758.5453123) * 5929);
	col_d = vec3_rotate_x(col_d, rand_seeded(seed) * 0.5 * M_PI * strength);
	seed = int(fract(sin(dot(d*0.82734, vec2(32.9898, 68.233))) * 33758.5453123) * 2929);
	col_d = vec3_rotate_z(col_d, rand_seeded(seed) * 2 * M_PI * 999999);

	vec3 r;

	r = mix(mix(col_c,col_d,easeInOutSine(pos.x)), mix(col_a,col_b,easeInOutSine(pos.x)), easeInOutSine(pos.y));
	return (normalize(r));
}
