vec2	get_uv_sphere(vec3 normal, bool inside)
{
	vec2	uv;

	if (inside == true)
		normal *= -1;
	uv.x = 0.5 + atan(normal.y, normal.x) / (2.0 * M_PI);
	uv.y = 0.5 - asin(-normal.z) / M_PI;
	return (uv);
}

void	calc_sphere_tangent_vectors(inout t_hitpoint hitpoint, bool inside)
{
	if (abs(hitpoint.normal.z) == 1)
	{
		hitpoint.tangent = vec3(hitpoint.normal.z, 0, 0);
		hitpoint.bitangent = vec3(0, hitpoint.normal.z, 0);
	}
	else
	{
		hitpoint.tangent = cross(vec3(0, 0, 1), hitpoint.normal);
		hitpoint.bitangent = cross(hitpoint.normal, hitpoint.tangent);
	}
	if (inside == true)
		hitpoint.bitangent *= -1;
}

t_hitpoint	get_hitpoint_sphere(t_ray ray, t_sphere sphere, bool init_all)
{
	t_hitpoint	hitpoint;
	float		discriminant;
	float		t0;
	float		t1;
	bool		inside = false;

	vec3 ap = ray.origin - sphere.origin;
	float a = dot(ray.dir, ray.dir);
	float b = 2 * dot(ap, ray.dir);
	discriminant = b * b - 4 * a * (dot(ap, ap) - sphere.radius * sphere.radius);
	if (discriminant < 0)
		return (HP_INF);

	float sqrt_discriminant = sqrt(discriminant);
	t1 = (-b - sqrt_discriminant) / (2 * a);
	if (t1 < 0)
	{
		t0 = (-b + sqrt_discriminant) / (2 * a);
		if (t0 < 0)
			return (HP_INF);
		inside = true;
		hitpoint.ray = t0 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		hitpoint.normal = normalize(sphere.origin - hitpoint.pos);
	}
	else
	{
		hitpoint.ray = t1 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		hitpoint.normal = normalize(hitpoint.pos - sphere.origin);
	}
	hitpoint.hit = true;
	hitpoint.color = sphere.base_color;
	hitpoint.material_idx = sphere.material_idx;

	if (init_all == false)
		return (hitpoint);
	
	// if we have an image_texture we calculate UVs
	if (has_image_texture(hitpoint))
		hitpoint.uv = get_uv_sphere(hitpoint.normal, inside);

	// We need tangent and bitangent vectors for normal_maps		
	if (has_normal_map_material(hitpoint))
	{
		calc_sphere_tangent_vectors(hitpoint, inside);
		hitpoint.normal = apply_normal_map(hitpoint);
	}

	return (hitpoint);
}

t_sphere	get_sphere(int offset)
{
	t_sphere	sphere;

	sphere.type = int(texelFetch(objects, offset++).r);
	sphere.next_offset = int(texelFetch(objects, offset++).r);
	sphere.origin = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	sphere.base_color = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	sphere.radius = texelFetch(objects, offset++).r;
	sphere.material_idx = int(texelFetch(objects, offset).r);

	return (sphere);
}

