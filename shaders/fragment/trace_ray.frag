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

// vec3 bounce2(vec3 normal)
// {
// 		float	z = mix(-1.0, 1.0, rand());
// 		float	radial_distance = sqrt(1 - z * z);
// 		float	phi = mix(0.0, 2 * M_PI, rand());
// 		float	x = radial_distance * cos(phi);
// 		float	y = radial_distance * sin(phi);
// 		vec3	bounce = vec3(x, y, z);

// 		if (dot(bounce, normal) < 0.0)
// 			bounce *= -1;
// 		return (bounce);
// }

vec3 bounce(vec3 normal)
{
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
	if (abs(normal.y) != 1)
		tangent = normalize(cross(vec3(0, 1, 0), normal));
	else
		tangent = normalize(cross(vec3(1, 0, 0), normal));
	vec3 bitangent = cross(normal, tangent);

	// Convert local space coordinates to world space
	vec3 bounce = normalize(x * tangent + y * bitangent + z * normal);

	return (bounce);
}

vec3 reflect(vec3 incoming, vec3 normal, float roughness)
{
	vec3 mirror = normalize(incoming - (2.0 * dot(incoming, normal) * normal));
	if (roughness == 0.0)
		return (mirror);
	vec3 bounce = bounce(normal);

	return (slerp(mirror, bounce, roughness));
}

float	lambert_diffuse(vec3 N, vec3 L)
{
	return (max(dot(N, L), 0.0));
}

vec3	fresnel(vec3 F0, vec3 V, vec3 H)
{
 	return ( F0 + (vec3(1.0) - F0) * pow((1 - max(dot(V, H), 0.0)), 5.0) );
}

float	normalDistribution(float a, vec3 N, vec3 H)
{
	// GGX Trowbridge-Reitz Normal Distribution Function
	float	numerator = pow(a, 2.0);

	float	NdotH = max(dot(N, H), 0.0);
	float	denominator = M_PI * pow(pow(NdotH, 2.0) * (pow(a, 2.0) - 1.0) + 1.0, 2.0);
	denominator = max(denominator, 0.0000001);

	return (numerator / denominator);
}

float	hacky_normalDistribution(float a, vec3 N, vec3 H)
{
	// GGX Trowbridge-Reitz Normal Distribution Function
	a = max(a, 0.00001);

	float aa = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float numerator = mix(pow(NdotH, 100000), aa, pow(a, 0.0001)); //incorrect hack to handle low roughness values
	numerator *= (1 + pow(a, 5)) * 2; //incorrect hack
	float denominator = M_PI * pow((NdotH * NdotH) * (aa - 1.0) + 1.0, 2.0);
	denominator = max(denominator, 0.0000001);

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

vec3	specular_cookTorrance(vec3 N, vec3 V, vec3 L, vec3 H, float a, vec3 ks, bool hacky_ndf)
{
	vec3	ct_numerator;
	if (hacky_ndf == true)
		ct_numerator = hacky_normalDistribution(a, N, H) * G(a, N, V, L) * ks;
	else
		ct_numerator = normalDistribution(a, N, H) * G(a, N, V, L) * ks;
	float	ct_denominator	= 4.0 * max(dot(V, N), 0.0) * max(dot(L, N), 0.0);
			ct_denominator	= max(ct_denominator, 0.000001);

	return	(ct_numerator / ct_denominator);
}

vec3	dielectric_F0(float IOR)
{
	// F0 is the base reflectance at normal incidence, determined from the IOR
	// this results in 0.04 when IOR is 1.5
	return (vec3(pow((IOR - 1) / (IOR + 1), 2)));
}

vec3	radiance(vec3 hit_pos, t_point_light point_light)
{
	float distance = distance(hit_pos, point_light.origin);
	return(point_light.color * (point_light.intensity / (distance * distance)));
}

vec3	get_point_light_contribution(vec3 hit_pos, t_point_light point_light, vec3 N, vec3 V, vec3 F0, float a, t_material mat)
{
	vec3 L  = normalize(point_light.origin - hit_pos);
	vec3 H  = normalize(V + L);
	vec3 ks = fresnel(F0, V, H);
	vec3 kd = (vec3(1.0) - ks) * (1.0 - mat.metallic);

	t_ray	light_ray;
	light_ray.dir = hit_pos - point_light.origin;
	light_ray.origin = point_light.origin;
	if (is_obstructed(light_ray) == true)
		return (VEC3_BLACK);

	float diffuse = lambert_diffuse(N, L);
	vec3  radiance = radiance(hit_pos, point_light);
	vec3  specular = vec3(0.0);
	if (mat.roughness > 0.0265 || point_light.radius > 0.0) // incorrect hack
		specular = specular_cookTorrance(N, V, L, H, a, ks, true);

	vec3 col = (kd * mat.color + specular) * radiance * diffuse;
	return (col);
}

vec3	get_reflection_light_contribution(vec3 hit_pos, vec3 reflection_col, vec3 N, vec3 V, vec3 L, t_material mat, vec3 previous_specular)
{
	vec3 F0 = mix(dielectric_F0(mat.ior), mat.color, mat.metallic) * 1.6; // incorrect hack
	vec3 H  = normalize(V + L);
	vec3 ks = fresnel(F0, V, H);
	vec3 kd = (vec3(1.0) - ks) * (1.0 - mat.metallic);
	float a = mat.roughness * mat.roughness;

	// float diffuse = lambert_diffuse(N, L);
	// vec3  radiance = radiance(hit_pos, point_light);
	vec3  specular = specular_cookTorrance(N, V, L, H, a, ks, true);
	specular = max(clamp(specular, vec3(0.0), ks), ks);

	specular *= previous_specular;
	out_hitpoint_color.rgb = specular; // out_hitpoint_color is actually out_hitpoint_specular_accumulated in this case

	// vec3 col = (kd * mat.color + specular) * radiance * diffuse;
	// vec3 col = (kd * mat.color + specular * reflection_col);
	vec3 col = min(specular * reflection_col, reflection_col);
	return (col);
}

vec3	get_ambient_light_contribution(vec3 hit_pos, t_hitpoint hitpoint, vec3 N, vec3 V, vec3 F0, float a, t_material mat)
{
	t_ray ray;
	ray.origin = hit_pos;
	ray.dir = reflect(hitpoint.ray, hitpoint.normal, pow(mat.roughness, 1.5));
	vec3 ambient_specular_light = get_sky_color_from_ray(ray);
	vec3 ambient_diffuse_light	= get_sky_color(hitpoint);

	vec3 L  = normalize(ray.dir);
	vec3 H  = normalize(V + L);
	vec3 ks = fresnel(F0, V, H);
	vec3 kd = (vec3(1.0) - ks) * (1.0 - mat.metallic);

	vec3 specular = specular_cookTorrance(N, V, L, H, a, ks, false);

	vec3 col = kd * mat.color * ambient_diffuse_light + max(clamp(specular, vec3(0.0), ks), ks) * ambient_specular_light;

	return (col);
}

vec3	render_hitpoint(t_hitpoint hitpoint)
{
	vec3 hit_pos = get_offset_hitpoint_pos(hitpoint);
	out_hitpoint_pos.rgb = hit_pos;
	out_hitpoint_normal.rgb = hitpoint.normal;
	out_hitpoint_normal.a = hitpoint.material_idx;
	vec3 col = VEC3_BLACK;
	t_material mat = materials[hitpoint.material_idx];

	// mat.color		= get_hitpoint_color(hitpoint);
	// hitpoint.color	= mat.color;
	mat.color		= hitpoint.color;
	mat.metallic	= get_hitpoint_metallic(hitpoint);
	mat.roughness	= get_hitpoint_roughness(hitpoint);
	if (rt.diffuse_bounce_count == 0 && rt.glossy_bounce_count == 0)
	{
		out_hitpoint_render.a		= mat.metallic;
		out_glossy_hitpoint_ray.a	= mat.roughness;
	}

	vec3  N   = hitpoint.normal;
	vec3  V   = normalize(-hitpoint.ray);
	vec3  F0p  = mix(dielectric_F0(mat.ior) * 1.6, mat.color, mat.metallic);
	vec3  F0  = mix(dielectric_F0(mat.ior), mat.color, mat.metallic);
	float a   = mat.roughness * mat.roughness;

	// POINT LIGHTS
	int i = -1;
	for (int type = next_light_type(i); type != LIGHT_NONE; type = next_light_type(i))
		col += get_point_light_contribution(hit_pos, get_point_light(i), N, V, F0p, a, mat);

	// AMBIENT LIGHT
	col += get_ambient_light_contribution(hit_pos, hitpoint, N, V, F0, a, mat);

	// EMISSION
	col += mat.emission_color * mat.emission_strength;

	return (clamp(col, 0, 16));
}

vec3	trace_ray(t_ray ray)
{
	vec3 col;
	t_hitpoint hitpoint = get_closest_hitpoint(ray, true);
	out_hitpoint_pos.a = float(hitpoint.hit);
	hitpoint.color = get_hitpoint_color(hitpoint);
	out_hitpoint_color.rgb = hitpoint.color;
	out_glossy_hitpoint_ray.rgb	= hitpoint.ray;

	if (hitpoint.hit == false)
		return (get_sky_color_from_ray(ray));
	col = render_hitpoint(hitpoint);

	return (col);
}

vec3	add_bounce_light(t_ray bounce_ray, t_hitpoint previous)
{
	vec3 col;
	t_hitpoint hitpoint = get_closest_hitpoint(bounce_ray, true);
	out_hitpoint_pos.a = float(hitpoint.hit);
	hitpoint.color = get_hitpoint_color(hitpoint);
	// out_hitpoint_color = hitpoint.color;
	if (hitpoint.hit == false)
		return (VEC3_BLACK);

	float distance = distance(hitpoint.pos, previous.pos);
	vec3  radiance = render_hitpoint(hitpoint);
	vec3 bounce_direction = normalize(previous.pos - hitpoint.pos);
	col = previous.color * radiance;
	out_hitpoint_color.rgb = previous.color * hitpoint.color;
	return (col);
}

vec3	add_reflection_light(t_ray reflection_ray, t_hitpoint previous, vec3 specular, float previous_metallic, float previous_roughness)
{
	vec3 col;
	t_hitpoint hitpoint = get_closest_hitpoint(reflection_ray, true);
	hitpoint.color = get_hitpoint_color(hitpoint);

	if (hitpoint.hit == false)
		col = get_sky_color_from_ray(reflection_ray);
	else
		col = render_hitpoint(hitpoint);

	t_material previous_material = materials[previous.material_idx];

	// previous_material.color		= get_hitpoint_color(previous);
	previous_material.metallic	= previous_metallic;
	previous_material.roughness	= previous_roughness;

	col = get_reflection_light_contribution(previous.pos, col, previous.normal, normalize(previous.ray), normalize(reflection_ray.dir), previous_material, specular);

	out_glossy_hitpoint_pos		= out_hitpoint_pos;
	out_glossy_hitpoint_pos.a 	= float(hitpoint.hit);
	out_glossy_hitpoint_normal	= out_hitpoint_normal;
	out_glossy_hitpoint_ray.rgb = hitpoint.ray;

	return (col);
}

// vec3	get_point_light_contribution2(vec3 hit_pos, t_point_light point_light, vec3 N, vec3 V, vec3 F0, float a, t_material mat)
// {
// 	vec3 L  = normalize(point_light.origin - hit_pos);
// 	vec3 H  = normalize(V + L);
// 	vec3 ks = fresnel(F0, V, H);
// 	vec3 kd = (vec3(1.0) - ks) * (1.0 - mat.metallic);

// 	t_ray	light_ray;
// 	light_ray.dir = hit_pos - point_light.origin;
// 	light_ray.origin = point_light.origin;
// 	if (is_obstructed(light_ray) == true)
// 		return (VEC3_BLACK);

// 	float diffuse = lambert_diffuse(N, L);
// 	vec3  radiance = radiance(hit_pos, point_light);
// 	vec3  specular = vec3(0.0);
// 	if (mat.roughness > 0.0265 || point_light.radius > 0.0) // incorrect hack
// 		specular = specular_cookTorrance(N, V, L, H, a, ks, true);

// 	vec3 col = (kd * mat.color + specular) * radiance;
// 	return (col);
// }

// vec3	get_bounce_portion_of_point_light(vec3 hit_pos, t_point_light point_light, vec3 N, vec3 V, vec3 F0, float a, t_material mat)
// {
// 	vec3 L  = normalize(point_light.origin - hit_pos);
// 	vec3 H  = normalize(V + L);
// 	vec3 ks = fresnel(F0, V, H);
// 	vec3 kd = (vec3(1.0) - ks) * (1.0 - mat.metallic);

// 	t_ray	light_ray;
// 	light_ray.dir = hit_pos - point_light.origin;
// 	light_ray.origin = point_light.origin;
// 	if (is_obstructed(light_ray) == true)
// 		return (VEC3_BLACK);

// 	float diffuse = lambert_diffuse(N, L);
// 	vec3  radiance = radiance(hit_pos, point_light);
// 	vec3  specular = vec3(0.0);
// 	if (mat.roughness > 0.0265 || point_light.radius > 0.0) // incorrect hack
// 		specular = specular_cookTorrance(N, V, L, H, a, ks, true);

// 	vec3 col = (kd * mat.color + specular) * radiance * diffuse * dot(N, V);
// 	return (col);
// }

// t_point_light	convert_hitpoint_into_point_light(t_hitpoint hitpoint)
// {
// 	t_point_light bounce_light;
// 	vec3 hit_pos = get_offset_hitpoint_pos(hitpoint);
// 	out_hitpoint_pos = hit_pos;
// 	out_hitpoint_normal = hitpoint.normal;
// 	out_hitpoint_misc.y = hitpoint.material_idx;
// 	vec3 col = VEC3_BLACK;
// 	t_material mat = materials[hitpoint.material_idx];

// 	mat.color = get_hitpoint_color(hitpoint);
// 	hitpoint.color = mat.color;

// 	vec3  N   = hitpoint.normal;
// 	vec3  V   = normalize(-hitpoint.ray);
// 	vec3  F0p = mix(dielectric_F0(mat.ior) * 1.6, mat.color, mat.metallic);
// 	vec3  F0  = mix(dielectric_F0(mat.ior), mat.color, mat.metallic);
// 	float a   = mat.roughness * mat.roughness;

// 	// POINT LIGHTS
// 	int i = -1;
// 	for (int type = next_light_type(i); type != LIGHT_NONE; type = next_light_type(i))
// 		col += get_bounce_portion_of_point_light(hit_pos, get_point_light(i), N, V, F0p, a, mat);

// 	// AMBIENT LIGHT
// 	col += get_ambient_light_contribution_no_specular(hitpoint);

// 	// EMISSION
// 	col += mat.emission_color * mat.emission_strength;

// 	bounce_light.type = LIGHT_POINT;
// 	bounce_light.origin = hitpoint.pos;
// 	bounce_light.color = col;
// 	// bounce_light.intensity = 1.0;
// 	bounce_light.intensity = 1.0 + 1.3;
// 	bounce_light.radius = 0.0;

// 	return (bounce_light);
// }
