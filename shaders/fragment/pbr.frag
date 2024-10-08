vec3	get_point_light_illumination(t_point_light point_light, t_hitpoint hitpoint)
{
	t_ray	light_ray;
	float	distance;
	float	intensity;

	light_ray.dir = get_offset_hitpoint_pos(hitpoint) - point_light.origin;
	light_ray.origin = point_light.origin;
	intensity = -dot(hitpoint.normal, normalize(light_ray.dir));
	if (intensity <= 0)
		return (VEC3_BLACK);
	distance = length(light_ray.dir);
	intensity = intensity / (distance * distance);
	if (intensity <= 0.000005 || is_obstructed(light_ray))
		return (VEC3_BLACK);
	intensity *= point_light.intensity;
	return (intensity * point_light.color);
}

vec3	lambert_diffuse(vec3 col)
{
	vec3	diffuse = col; // / M_PI;

	return (diffuse);
}

vec3	fresnel(vec3 F0, vec3 V, vec3 H)
{
 	return ( F0 + (vec3(1) - F0) * pow((1 - max(dot(V, H), 0.0)), 5.0) );
}

float	normalDistribution(float a, vec3 N, vec3 H)
{
	// GGX Trowbridge-Reitz Normal Distribution Function
	float	numerator = pow(a, 2.0);

	float	NdotH = max(dot(N, H), 0.0);
	float	denominator = M_PI * pow(pow(NdotH, 2.0) * (pow(a, 2.0) - 1.0) + 1.0, 2.0);
	denominator = max(denominator, 0.000001);

	return (numerator / denominator);
}

float	G1(float a, vec3 N, vec3 X)
{
	// Schlick-Beckmann Geometry Shadowing Function
	float	numerator = max(dot(N, X), 0.0);

	float	k = a / 2.0;
	float	denominator = max(dot(N, X), 0.0) * (1.0 - k) + k;
	denominator = max(denominator, 0.000001);

	return (numerator / denominator);
}

float	G(float a, vec3 N, vec3 V, vec3 L)
{
	// SMITH MODEL
	return ( G1(a, N, V) * G1(a, N, L) );
}

vec3	specular_cookTorrance(vec3 N, vec3 V, vec3 L, vec3 H, float a, vec3 ks)
{
	vec3	ct_numerator	= normalDistribution(a, N, H) * G(a, N, V, L) * ks;
	float	ct_denominator	= 4.0 * max(dot(V, N), 0.0) * max(dot(L, N), 0.0);
			ct_denominator	= max(ct_denominator, 0.000001);

	return	(ct_numerator / ct_denominator);
}

vec3	point_light_brdf(t_hitpoint hitpoint, t_point_light point_light, vec3 N, vec3 V, vec3 L, vec3 H)
{
	vec3	col;
	float	metallic = materials[hitpoint.material_idx].metallic;

	vec3	F0 = mix(vec3(0.04), hitpoint.color, metallic);

	float	roughness = materials[hitpoint.material_idx].roughness;
	float	a = roughness * roughness;

	vec3	ks = fresnel(F0, V, H);
	vec3	kd = (vec3(1) - ks) * (1.0 - metallic);

	vec3	diffuse;
	// Lambert  Diffuse
	diffuse = lambert_diffuse(hitpoint.color);
	// Oren Nayar Diffuse
	// diffuse = oren_nayar_diffuse(hitpoint.color, N, V, L, a);

	vec3	specular = specular_cookTorrance(N, V, L, H, a, ks);

	vec3	BRDF = kd * diffuse + specular;

	col = BRDF * get_point_light_illumination(point_light, hitpoint);

	return (col);
}

vec3	compute_pbr(t_ray ray)
{
	int 		i;
	float		type;
	vec3		col = VEC3_BLACK;
	t_hitpoint	hitpoint;

	hitpoint = get_closest_hitpoint(ray, true);
	hitpoint.color = get_hitpoint_color(hitpoint);

	if (hitpoint.hit)
	{
		vec3	N = hitpoint.normal;
		vec3	V = -normalize(hitpoint.ray);

		i = -1;
		type = next_light_type(i);
		while (type != LIGHT_NONE)
		{
			t_point_light point_light = get_point_light(i);

			vec3	L = normalize(point_light.origin - hitpoint.pos);
			vec3	H = normalize(V + L);

			t_ray	light_ray;
			light_ray.dir = get_offset_hitpoint_pos(hitpoint) - point_light.origin;
			light_ray.origin = point_light.origin;
			if (is_obstructed(light_ray) == false)
				col += point_light_brdf(hitpoint, point_light, N, V, L, H);

			type = next_light_type(i);
		}
	}
	else
		return (get_sky_color_from_ray(ray, hitpoint));

	return (col);
}
