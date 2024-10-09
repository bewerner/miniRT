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

vec3	slerp(vec3 v0, vec3 v1, float t)
{
	float	dp;
	dp = dot(v0, v1);
	dp = clamp(dp, -1.0, 1.0);

	float	angle;
	angle = acos(dp) * t;

	vec3	orthonormal;
	orthonormal = normalize(v1 - v0 * dp);

	return ( normalize(v0 * cos(angle) + orthonormal * sin(angle)) );
}

vec3 get_random_cosine_weighted_hemisphere_direction_roughness_dependent(t_hitpoint hitpoint)
{
	// Get reflection ray
	vec3	reflection_dir;
	reflection_dir = normalize(create_bounce_dir(hitpoint.ray, hitpoint.normal));

	// Generate random numbers in the range [0, 1]
	float u = rand();
	float v = rand();

	// Convert random numbers to spherical coordinates
	float theta = acos(sqrt(u)); // Cosine weighted
	float phi = 2.0 * M_PI * v; // Uniformly distributed in [0, 2π]

	// Spherical to Cartesian coordinates
	float x = sin(theta) * cos(phi);
	float y = sin(theta) * sin(phi);
	float z = cos(theta);

	// Create a local tangent basis for the hemisphere
	vec3 tangent;
	if (abs(hitpoint.object_normal.y) != 1)
		tangent = normalize(cross(vec3(0, 1, 0), hitpoint.object_normal));
	else
		tangent = normalize(cross(vec3(1, 0, 0), hitpoint.object_normal));
	vec3 bitangent = cross(hitpoint.object_normal, tangent);

	// Convert local space coordinates to world space
	vec3 random_direction = normalize(x * tangent + y * bitangent + z * hitpoint.object_normal);

	// Slerp
	return ( slerp(reflection_dir, random_direction, materials[hitpoint.material_idx].roughness) );

	// Lerp
	// return ( mix(reflection_dir, random_direction, materials[hitpoint.material_idx].roughness) );
}

vec3	lambert_diffuse(vec3 col)
{
	vec3	diffuse = col; // / M_PI;

	return (diffuse);
}

// vec3	oren_nayar_diffuse(vec3 col, vec3 N, vec3 V, vec3 L, float a)
// {
// 	vec3	ond;

// 	float	cos_thetaI = max(dot(N, L), 0.0);
// 	float	cos_thetaO = max(dot(N, V), 0.0);

// 	float	thetaI = acos(cos_thetaI);
// 	float	thetaO = acos(cos_thetaO);

// 	vec3	projectedL = normalize(L - dot(N, L) * N);
// 	vec3	projectedV = normalize(V - dot(N, V) * N);

// 	float	phiDiff = max(dot(projectedL, projectedV), 0.0);

// 	// Oren Nayar A and B terms
// 	float	A = (1 - a) / 2 * (a + 0.33);
// 	float	B = (0.45 * a) / (a + 0.09);

// 	ond = col * ( A + B * phiDiff * sin(thetaI) * tan(thetaO)); // / M_PI;
// 	return (ond);
// }

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

vec3	calculate_F0(float IOR)
{
	// F0 is the base reflectance at normal incidence, determined from the IOR
	// this results in 0.04 when IOR is 1.5
	return (vec3(pow((IOR - 1) / (IOR + 1), 2)));
}

vec3	point_light_brdf(t_hitpoint hitpoint, t_point_light point_light, vec3 N, vec3 V, vec3 L, vec3 H)
{
	vec3	col;
	float	metallic = materials[hitpoint.material_idx].metallic;
	float	IOR = materials[hitpoint.material_idx].ior;

	vec3	F0 = mix(calculate_F0(IOR), hitpoint.color, metallic);

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

vec3	ambient_brdf(t_hitpoint hitpoint, vec3 N, vec3 V)
{
	vec3	col;

	// RECIEVE AMBIENT
	t_ray	ray;
	vec3	ambient_diffuse_light;
	vec3	ambient_specular_light;

	ray.dir 				= get_real_random_hemisphere_direction(hitpoint);
	ambient_diffuse_light	= get_sky_color_from_ray(ray);
	ambient_diffuse_light	= get_sky_color(hitpoint);

	ray.origin				= get_offset_hitpoint_pos(hitpoint);
	ray.dir					= get_random_cosine_weighted_hemisphere_direction_roughness_dependent(hitpoint);
	ambient_specular_light	= get_sky_color_from_ray(ray);

	// vec3	ambient_light;
	// ambient_light = (ambient_diffuse_light + ambient_specular_light) / 2;

	// vec3	N = hitpoint.normal;
	// vec3	V = -normalize(hitpoint.ray);
	vec3	L = normalize(ray.dir);
	vec3	H = normalize(V + L);

	float	metallic = materials[hitpoint.material_idx].metallic;
	float	IOR = materials[hitpoint.material_idx].ior;

	vec3	F0 = mix(calculate_F0(IOR), hitpoint.color, metallic);

	float	roughness = materials[hitpoint.material_idx].roughness;
	float	a = roughness * roughness;

	vec3	ks = fresnel(F0, V, H);
	vec3	kd = (vec3(1) - ks) * (1.0 - metallic);

	vec3	diffuse;
	// Lambert  Diffuse
	diffuse = lambert_diffuse(hitpoint.color);
	// Oren Nayar Diffuse
	// diffuse = oren_nayar_diffuse(hitpoint.color, N, V, L, a);

	float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);

	vec3	specular = specular_cookTorrance(N, V, L, H, a, ks);
	// specular = ambient_light;

	vec3	BRDF;
	// if (rt.debug == -1)
	// 	BRDF = kd * diffuse + specular * max(dot(N, L), 0.0);
	// else
		// BRDF = kd * diffuse + specular * NdotL * NdotV;

	// BRDF = kd * diffuse + specular;


	// BRDF = (kd * diffuse * ambient_diffuse_light + ambient_specular_light) / 2; //old
	// BRDF = kd * diffuse * ambient_diffuse_light + ks * ambient_specular_light; // new
	BRDF = kd * diffuse * ambient_diffuse_light + max(clamp(specular, 0.0, 1.0), ks) * ambient_specular_light; // new new
		// of course this makes no sense, but it might point out the issue. probably specular_cookTorrance needs
		// to be adjusted so that in the final version we can do something like
		// kd * diffuse * ambient_diffuse_light + specular * ambient_specular_light;

		// seems like the specular_cookTorrance produces values bigger than 1.0
		// this goes against the whole conservation of energy principle i think?


	// BRDF = mix(kd * diffuse * ambient_diffuse_light, specular * ambient_specular_light, NdotV);

	// BRDF = specular;

	// if (BRDF.r <= vec3(0.75).r)
	// 	BRDF = vec3(1,0,0);

	// col = BRDF * ambient_light;
	col = BRDF;
	// col += max(max(rt.ambient, 0.0), 0.0) * NdotV;
	// col /= 2;

	// if (rt.debug == -1)
	// 	col = ambient_diffuse_light;
	// else if (rt.debug == -2)
	// 	col = ambient_specular_light;
	// else if (rt.debug == -3)
	// 	col = specular;
	// if (specular.r > 1.0 || specular.g > 1.0 || specular.b > 1.0)
	// 	col = vec3(1,0,0);

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

		// AMBIENT - is this necessary or does globel/indirect illumination solve it for free?
		col += ambient_brdf(hitpoint, N, V);

		// EMISSION
		col += materials[hitpoint.material_idx].emission_color * materials[hitpoint.material_idx].emission_strength;

	}
	else
		return (get_sky_color_from_ray(ray));

	return (col);
}
