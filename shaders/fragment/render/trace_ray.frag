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

float	lambert_diffuse(vec3 N, vec3 L)
{
	return (max(dot(N, L), 0.0));
}

vec3	fresnel(vec3 F0, vec3 V, vec3 H)
{
 	return ( F0 + (vec3(1.0) - F0) * pow((1 - max(dot(V, H), 0.0)), 5.0) );
}

float	normalDistribution3(float a, vec3 N, vec3 H)
{
	a = max(a, 0.1);

	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float r = a2 / (M_PI * pow((1.0 - NdotH2) + (a2 * NdotH2) , 2));
	return (r);
}

// float	normalDistribution(float a, vec3 N, vec3 H)
// {
// 	// GGX Trowbridge-Reitz Normal Distribution Function
// 	float aa = a * a;
// 	float numerator = aa;
// 	float NdotH = max(dot(N, H), 0.0);
// 	float denominator = M_PI * pow((NdotH * NdotH) * (aa - 1.0) + 1.0, 2.0);
// 	denominator = max(denominator, 0.0000001);

// 	float r = numerator / denominator;
// 	return (r);
// }

float	normal_distribution(float a, vec3 N, vec3 H)
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

float	geometry_shadowing(float a, vec3 N, vec3 V, vec3 L)
{
	// SMITH MODEL
	return (G1(a, N, V) * G1(a, N, L));
}

vec3	cook_torrance(vec3 N, vec3 V, vec3 L, vec3 H, float a, vec3 ks)
{
	vec3	ct_numerator = normal_distribution(a, N, H) * geometry_shadowing(a, N, V, L) * ks;
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
	vec3 radiance = radiance(hit_pos, point_light);
	vec3 specular = vec3(0.0);
	if (mat.roughness > 0.0265 || point_light.radius > 0.0) // incorrect hack to make it so surfaces with roughness=0 don't have specular hightlights of point-lights with radius=0
		specular = cook_torrance(N, V, L, H, a, ks);

	vec3 col = (kd * mat.color + specular) * radiance * diffuse;
	return (col);
}

float get_pdf_reflection(vec3 V, vec3 N, vec3 L, float roughness)
{
	vec3  H = normalize(V + L);

	float a = roughness * roughness;

	float D = normal_distribution(a, N, H);
	float G = geometry_shadowing(a, N, V, L);

	float NdotV = max(dot(N, V), 0.0);
	float HdotV = max(dot(H, V), 0.0);

	float pdf = (D * G * HdotV) / (4.0 * NdotV);

	return (pdf);
}

vec3	get_ambient_light_contribution(vec3 hit_pos, t_hitpoint hitpoint, vec3 N, vec3 V, vec3 F0, float a, t_material mat)
{
	t_ray	ray_importance;
	t_ray	ray_cosine;
	t_ray	ray_reflection;
	float	weight_importance_diffuse;
	float	weight_importance_specular;
	float	pdf_importance;
	float	pdf_cosine;
	float	pdf_reflection;
	vec3	radiance_importance;
	vec3	col = VEC3_BLACK;

	vec3	specular_reflectance;
	vec3	diffuse_reflectance;

	ray_cosine.origin = hit_pos;
	ray_cosine.dir = sample_hemisphere(N);
	pdf_cosine = dot(N, ray_cosine.dir) / M_PI;
	pdf_cosine = max(pdf_cosine, 1e-6);

	ray_reflection.origin = hit_pos;
	ray_reflection.dir = reflect(-V, N, hitpoint.object_normal, mat.roughness);
	pdf_reflection = get_pdf_reflection(V, N, ray_reflection.dir, mat.roughness);
	pdf_reflection = max(pdf_reflection, 1e-6);

	ray_importance.origin = hit_pos;
	ray_importance.dir = sample_environment_map(pdf_importance, radiance_importance);

	weight_importance_diffuse  = pdf_importance / (pdf_importance + pdf_cosine);
	weight_importance_specular = pdf_importance / (pdf_importance + pdf_reflection);
	weight_importance_diffuse  *= rt.env_weight_adjustment;
	weight_importance_specular *= rt.env_weight_adjustment;

	{
		vec3 L  = ray_reflection.dir;
		vec3 H  = normalize(V + L);
		specular_reflectance = fresnel(F0, V, H);
		diffuse_reflectance = (1.0 - specular_reflectance) * (1.0 - mat.metallic);
	}

	if (reaches_sky(ray_cosine) == true)
	{
		vec3 L  = ray_cosine.dir;
		vec3 H  = normalize(V + L);
		vec3 ks = mix(specular_reflectance, fresnel(F0, V, H), mat.roughness);
		vec3 kd = (1.0 - ks) * (1.0 - mat.metallic);
		float NdotL = max(0.0, dot(N, L));
		vec3 radiance = get_ambient_color(L) * (1.0 - weight_importance_diffuse);
		col += kd * mat.color * radiance;
	}

	if (pdf_importance > 1e-6 && reaches_sky(ray_importance) == true)
	{
		vec3 L  = ray_importance.dir;
		vec3 H  = normalize(V + L);
		vec3 ks = mix(specular_reflectance, fresnel(F0, V, H), mat.roughness);
		vec3 kd = (1.0 - ks) * (1.0 - mat.metallic);
		float NdotL = max(0.0, dot(N, L));
		vec3 radiance_diffuse  = radiance_importance * weight_importance_diffuse;
		vec3 radiance_specular = radiance_importance * weight_importance_specular;
		vec3 specular = cook_torrance(N, V, L, H, a, ks);
		col += (kd * mat.color * radiance_diffuse * NdotL);
		col += (specular * radiance_specular * NdotL * (1.0 + cos((mat.roughness - 0.5) * (M_PI / 2))));
	}

	if (reaches_sky(ray_reflection) == true)
	{
		vec3 L  = ray_reflection.dir;
		vec3 H  = normalize(V + L);
		vec3 ks = specular_reflectance;
		float NdotL = max(0.0, dot(N, L));
		vec3 radiance = get_ambient_color(L);
		vec3 specular = cook_torrance(N, V, L, H, a, ks);
		specular = max(clamp(specular, vec3(0.0), ks), ks);
		col += specular * radiance * mix(1.0, NdotL, mat.roughness * (1.0 - mat.metallic));
	}

	return (col);
}

vec3	get_reflection_light_contribution(vec3 hit_pos, vec3 reflection_col, vec3 N, vec3 V, vec3 L, t_material mat, vec3 previous_specular)
{
	vec3 F0 = mix(dielectric_F0(mat.ior), mat.color, mat.metallic) * (((1.6))); // incorrect hack
	vec3 H  = normalize(V + L);
	vec3 ks = fresnel(F0, V, H);
	vec3 kd = (vec3(1.0) - ks) * (1.0 - mat.metallic);
	float a = mat.roughness * mat.roughness;

	vec3 specular = cook_torrance(N, V, L, H, a, ks);
	specular = max(clamp(specular, vec3(0.0), ks), ks);

	specular *= previous_specular;
	out_specular.rgb = specular;

	vec3 col = min(specular * reflection_col, reflection_col);
	return (col);
}

vec3	render_hitpoint(t_hitpoint hitpoint)
{
	vec3 hit_pos = get_offset_hitpoint_pos(hitpoint);
	vec3 col = VEC3_BLACK;
	t_material mat = materials[hitpoint.material_idx];

	mat.color		= hitpoint.color;
	mat.metallic	= get_hitpoint_metallic(hitpoint);
	mat.roughness	= get_hitpoint_roughness(hitpoint);

	vec3  N   = hitpoint.normal;
	vec3  V   = normalize(-hitpoint.ray);
	vec3  F0p  = mix(dielectric_F0(mat.ior) * 1.6, mat.color, mat.metallic);
	vec3  F0  = mix(dielectric_F0(mat.ior), mat.color, mat.metallic);
	float a   = mat.roughness * mat.roughness;

	int i = -1;
	for (int type = next_light_type(i); type != LIGHT_NONE; type = next_light_type(i))
		col += get_point_light_contribution(hit_pos, get_point_light(i), N, V, F0p, a, mat);

	col += get_ambient_light_contribution(hit_pos, hitpoint, N, V, F0, a, mat);

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
	mat.roughness	= 1;

	vec3  N   = hitpoint.normal;
	vec3  V   = normalize(-hitpoint.ray);
	vec3  F0  = dielectric_F0(mat.ior);
	float a   = 1;

	int i = -1;
	for (int type = next_light_type(i); type != LIGHT_NONE; type = next_light_type(i))
		col += get_point_light_contribution(hit_pos, get_point_light(i), N, V, F0 * 1.6, a, mat);

	col += get_ambient_light_contribution(hit_pos, hitpoint, N, V, F0, a, mat);

	col += mat.emission_color * mat.emission_strength;

	return (clamp(col, 0, 16));
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

	previous_material.metallic	= previous_metallic;
	previous_material.roughness	= previous_roughness;

	col = get_reflection_light_contribution(previous.pos, col, previous.normal, normalize(previous.ray), normalize(reflection_ray.dir), previous_material, specular);

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
		return (get_ambient_color(ray.dir));
	col = render_hitpoint(hitpoint);

	return (col);
}
