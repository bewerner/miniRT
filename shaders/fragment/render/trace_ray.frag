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
	float u = rand();
	float v = rand();

	float theta = acos(sqrt(u));
	float phi = 2.0 * M_PI * v;

	float x = sin(theta) * cos(phi);
	float y = sin(theta) * sin(phi);
	float z = cos(theta);

	vec3 tangent;
	if (abs(normal.y) != 1)
		tangent = normalize(cross(vec3(0, 1, 0), normal));
	else
		tangent = normalize(cross(vec3(1, 0, 0), normal));
	vec3 bitangent = cross(normal, tangent);

	vec3 bounce = normalize(x * tangent + y * bitangent + z * normal);

	return (bounce);
}

vec3 sample_half_vector(vec3 normal, float roughness)
{
	float u = rand() * 1.1;
	float v = rand() * 1.1;

	float theta = acos(sqrt(u));
	float phi = 2.0 * M_PI * v;

	float x = sin(theta) * cos(phi);
	float y = sin(theta) * sin(phi);
	float z = cos(theta);

	vec3 tangent;
	if (abs(normal.y) != 1)
		tangent = normalize(cross(vec3(0, 1, 0), normal));
	else
		tangent = normalize(cross(vec3(1, 0, 0), normal));
	vec3 bitangent = cross(normal, tangent);

	vec3 bounce = normalize(x * tangent + y * bitangent + z * normal);

	return (bounce);
}

vec3 mirror(vec3 incoming, vec3 normal)
{
	return (normalize(incoming - (2.0 * dot(incoming, normal) * normal)));
}

vec3 reflect(vec3 incoming, vec3 normal, float roughness)
{
	vec3 reflection;

	if (roughness == 0.0)
		reflection = mirror(incoming, normal);
	else if (roughness == 1.0)
		reflection = bounce(normal);
	else
	{
		reflection = mirror(incoming, normal);
		reflection = slerp(reflection, bounce(reflection), roughness);
	}

	return (reflection);
}

// vec3 reflect(vec3 incoming, vec3 normal, vec3 object_normal, float roughness)
// {
// 	vec3 reflection;

// 	if (rt.debug < 0)
// 		roughness = -rt.debug/100;



// 	if (roughness == 0.0)
// 		reflection = mirror(incoming, normal);
// 	else if (roughness == 1.0)
// 		reflection = bounce(normal);
// 	else
// 	{


// 		vec3 F0 = 0.04; // incorrect hack
// 		vec3 N  = normal;
// 		vec3 V  = normalize(-incoming);
// 		vec3 L  = mirror(incoming, normal);
// 		vec3 H  = normalize(V + L);
// 		vec3 ks = fresnel(F0, V, H);
// 		// vec3 kd = (vec3(1.0) - ks) * (1.0 - mat.metallic);
// 		vec3 kd = vec3(1.0) - ks;
// 		float a = roughness * roughness;
// 		vec3  specular = specular_cookTorrance(N, V, L, H, a, ks, true);

// 		reflection = normalize(specular);

// 		// reflection = mirror(incoming, normal);
// 		// reflection = slerp(reflection, bounce(reflection), roughness);
// 	}
	

// 	// if (dot(reflection, object_normal) < 0)
// 	// 	reflection = reflect(reflection, object_normal, roughness);

// 	return (reflection);
// }

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

float	geometryShadowing(float a, vec3 N, vec3 V, vec3 L)
{
	// SMITH MODEL
	return ( G1(a, N, V) * G1(a, N, L) );
}

vec3	specular_cookTorrance(vec3 N, vec3 V, vec3 L, vec3 H, float a, vec3 ks, bool hacky_ndf)
{
	vec3	ct_numerator;
	if (hacky_ndf == true)
		ct_numerator = hacky_normalDistribution(a, N, H) * geometryShadowing(a, N, V, L) * ks;
	else
		ct_numerator = normalDistribution(a, N, H) * geometryShadowing(a, N, V, L) * ks;
	float	ct_denominator	= 4.0 * max(dot(V, N), 0.0) * max(dot(L, N), 0.0);
			ct_denominator	= max(ct_denominator, 0.000001);

	return	(ct_numerator / ct_denominator);
}

vec3	specular_cookTorrance2(vec3 N, vec3 V, vec3 L, vec3 H, float a, vec3 ks, bool hacky_ndf)
{
	vec3	ct_numerator;
	if (hacky_ndf == true)
		ct_numerator = ks;
	else
		ct_numerator = ks;
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
	out_specular.rgb = specular;

	// vec3 col = (kd * mat.color + specular) * radiance * diffuse;
	// vec3 col = (kd * mat.color + specular * reflection_col);
	vec3 col = min(specular * reflection_col, reflection_col);
	return (col);
}

vec3	get_ambient_light_contribution(vec3 hit_pos, t_hitpoint hitpoint, vec3 N, vec3 V, vec3 F0, float a, t_material mat)
{
	t_ray ray;

	// vec3 H = sample_half_vector(N, mat.roughness);
	// vec3 L = mirror(-V, H);


	ray.origin = hit_pos;
	// ray.dir = reflect(hitpoint.ray, hitpoint.normal, hitpoint.object_normal, pow(mat.roughness, 1.5));
	ray.dir = reflect(hitpoint.ray, hitpoint.normal, hitpoint.object_normal, mat.roughness);
	// ray.dir = L;
	vec3 ambient_specular_light = clamp(get_sky_color_from_ray(ray), 0, 16);
	vec3 ambient_diffuse_light	= get_sky_color(hitpoint);

	// return (ambient_specular_light);

	vec3  L = normalize(ray.dir);
	vec3  H = normalize(V + L);

	float G = geometryShadowing(a, N, V, L);
	vec3  F = fresnel(F0, V, H);
	float D = normalDistribution(a, N, H);

	vec3 ks = max(F, clamp(F * D * G, vec3(0.0), F));
	vec3 kd = (vec3(1.0) - F);// * (1.0 - mat.metallic);
	// if (rt.debug < 0)
	// 	kd = (vec3(1.0) - ks) * (1.0 - mat.metallic);

	// return (kd * mat.color * ambient_diffuse_light + ks * ambient_specular_light);

	vec3 specular = specular_cookTorrance2(N, V, L, H, a, ks, false);

	vec3 col;
	col = kd * mat.color * ambient_diffuse_light + max(clamp(specular, vec3(0.0), F), F) * ambient_specular_light;
	// if (rt.debug < 0)
	// 	col = kd * mat.color * ambient_diffuse_light + clamp(specular, vec3(0.0), ks) * ambient_specular_light;

	if (rt.debug == 1)
		return (ks);
	if (rt.debug == 2)
		return (kd);
	if (rt.debug == 3)
		return (specular);
	if (rt.debug == 4)
		return (ambient_specular_light);
	if (rt.debug == 5)
		return (ambient_diffuse_light);
	return (clamp(col, 0, 16));
}

vec3	render_hitpoint(t_hitpoint hitpoint)
{
	vec3 hit_pos = get_offset_hitpoint_pos(hitpoint);
	vec3 col = VEC3_BLACK;
	t_material mat = materials[hitpoint.material_idx];

	// mat.color		= get_hitpoint_color(hitpoint);
	// hitpoint.color	= mat.color;
	mat.color		= hitpoint.color;
	mat.metallic	= get_hitpoint_metallic(hitpoint);
	mat.roughness	= get_hitpoint_roughness(hitpoint);

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

vec3	get_bounce_light(t_hitpoint hitpoint)
{
	vec3 hit_pos = get_offset_hitpoint_pos(hitpoint);
	vec3 col = VEC3_BLACK;
	t_material mat = materials[hitpoint.material_idx];

	mat.color		= hitpoint.color;
	mat.metallic	= 0;
	mat.roughness	= 0;

	vec3  N   = hitpoint.normal;
	vec3  V   = normalize(-hitpoint.ray);
	vec3  F0  = dielectric_F0(mat.ior);
	float a   = 0;

	// POINT LIGHTS
	int i = -1;
	for (int type = next_light_type(i); type != LIGHT_NONE; type = next_light_type(i))
		col += get_point_light_contribution(hit_pos, get_point_light(i), N, V, F0 * 1.6, a, mat);

	// AMBIENT LIGHT
	col += get_ambient_light_contribution(hit_pos, hitpoint, N, V, F0, a, mat);

	// EMISSION
	col += mat.emission_color * mat.emission_strength;

	return (clamp(col, 0, 16));
}

vec3	trace_camera_ray(t_ray ray)
{
	vec3 col;
	t_hitpoint hitpoint = get_closest_hitpoint(ray, true);
	out_camera_ray_dir.a = float(hitpoint.object_idx);
	hitpoint.color = get_hitpoint_color(hitpoint);
	out_diffuse.rgb = hitpoint.color * (1.0 - get_hitpoint_metallic(hitpoint));
	out_specular.rgb = hitpoint.color;

	if (hitpoint.hit == false)
		return (get_sky_color_from_ray(ray));
	col = render_hitpoint(hitpoint);

	return (col);
}

vec3	trace_bounce_ray(t_ray bounce_ray, t_hitpoint previous)
{
	vec3 diffuse = out_diffuse.rgb;
	if (diffuse == VEC3_BLACK)
	{
		out_previous_ray_dir.a = -1;
		return (VEC3_BLACK);
	}
	t_hitpoint hitpoint = get_closest_hitpoint(bounce_ray, true);
	out_previous_ray_dir.a = float(hitpoint.object_idx);
	if (hitpoint.hit == false)
		return (VEC3_BLACK);
	hitpoint.color = get_hitpoint_color(hitpoint);
	out_diffuse.rgb *= hitpoint.color * (1.0 - get_hitpoint_metallic(hitpoint));

	vec3 bounce_light = get_bounce_light(hitpoint);

	return (diffuse * bounce_light);
}

vec3	trace_reflection_ray(t_ray reflection_ray, t_hitpoint previous, vec3 specular, float previous_metallic, float previous_roughness)
{
	vec3 col;
	t_hitpoint hitpoint = get_closest_hitpoint(reflection_ray, true);
	out_previous_ray_dir.a = float(hitpoint.object_idx);
	if (hitpoint.hit == false)
		return vec3(0);

	hitpoint.color = get_hitpoint_color(hitpoint);

	col = render_hitpoint(hitpoint);

	t_material previous_material = materials[previous.material_idx];

	// previous_material.color		= get_hitpoint_color(previous);
	previous_material.metallic	= previous_metallic;
	previous_material.roughness	= previous_roughness;

	col = get_reflection_light_contribution(previous.pos, col, previous.normal, normalize(previous.ray), normalize(reflection_ray.dir), previous_material, specular);

	return (clamp(col, 0, 16));
}

vec3 sampleGGXNormal(vec3 N, float roughness) {
    // Step 1: Convert roughness to GGX alpha parameter
    float alpha = roughness * roughness;

    // Step 2: Generate random values for spherical coordinates (u1 and u2)
    float u1 = rand();  // Replace rand() with a random number generator (0 to 1)
    float u2 = rand();

    // Step 3: Compute theta and phi for GGX distribution
    float theta = atan(alpha * sqrt(u1 / (1.0 - u1)));
    float phi = 2.0 * M_PI * u2;

    // Step 4: Convert spherical coordinates to Cartesian (tangent space)
    float x = sin(theta) * cos(phi);
    float y = sin(theta) * sin(phi);
    float z = cos(theta);

    // Step 5: Create the tangent and bitangent vectors for N
    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    // Step 6: Transform the sample to world space
    vec3 H = x * tangent + y * bitangent + z * N;
    return normalize(H);  // Ensure the sampled normal is normalized
}

vec3 reflect(vec3 incoming, vec3 normal, vec3 object_normal, float roughness)
{
	vec3 reflection;

	incoming = normalize(incoming);
	// if (rt.debug < 0)
	// 	roughness = -rt.debug/100;



	if (roughness == 0.0)
		reflection = mirror(incoming, normal);
	// else if (roughness == 1.0)
	// 	reflection = bounce(normal);
	else
	{
		// float D = normalDistribution(roughness, normal, H);

		// vec3 H = sample_half_vector(normal, roughness);
		// // reflection = sample_half_vector(normal, roughness);
		// reflection = mirror(incoming, H);
		// if (dot(H, normal) < 0)
		// 	reflection = mirror(incoming, normal);

		// reflection = mirror(incoming, normal);
		// reflection = normalize(slerp(reflection, bounce(reflection), roughness));

		vec3 new_normal = normalize(slerp(normal, bounce(normal), roughness*roughness));
		reflection = mirror(incoming, new_normal);
		if (dot(reflection, object_normal) < 0)
			reflection = mirror(reflection, object_normal);
		// if (dot(reflection, object_normal) < 0 && rt.debug == -1)
		// 	reflection = mirror(incoming, object_normal);
		// if (dot(reflection, object_normal) < 0 && rt.debug == -2)
		// 	reflection = mirror(reflection, object_normal);





		// vec3 H = sampleGGXNormal(normal, roughness);

		// reflection = normalize(incoming - (2.0 * dot(incoming, normal) * H));
		// reflection = normalize(reflection);

		// reflection = mirror(incoming, normal);
		// reflection = slerp(reflection, bounce(reflection), roughness);
	}
	

	// if (dot(reflection, object_normal) < 0)
	// 	reflection = reflect(reflection, object_normal, roughness);

	return (reflection);
}

// vec3 reflect(vec3 incoming, vec3 normal, vec3 object_normal, float roughness)
// {
// 	vec3 reflection;

// 	if (rt.debug < 0)
// 		roughness = -rt.debug/100;



// 	if (roughness == 0.0)
// 		reflection = mirror(incoming, normal);
// 	else if (roughness == 1.0)
// 		reflection = bounce(normal);
// 	else
// 	{


// 		vec3 F0 = vec3(0.04);
// 		vec3 N  = normal;
// 		vec3 V  = normalize(-incoming);
// 		vec3 L  = mirror(incoming, normal);
// 		vec3 H  = normalize(V + L);
// 		vec3 ks = fresnel(F0, V, H);
// 		// vec3 kd = (vec3(1.0) - ks) * (1.0 - mat.metallic);
// 		vec3 kd = vec3(1.0) - ks;
// 		float a = roughness * roughness;
// 		vec3  specular = specular_cookTorrance(N, V, L, H, a, ks, false);

// 		float factor = dot(specular, vec3(1)) / 3;

// 		reflection = slerp(mirror(incoming, normal), bounce(normal), factor);
// 		// reflection = normalize(reflection);

// 		// reflection = mirror(incoming, normal);
// 		// reflection = slerp(reflection, bounce(reflection), roughness);
// 	}
	

// 	// if (dot(reflection, object_normal) < 0)
// 	// 	reflection = reflect(reflection, object_normal, roughness);

// 	return (reflection);
// }
