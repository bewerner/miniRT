vec2	get_uv_sphere(t_sphere sphere, vec3 normal, bool inside)
{
	vec2	uv;

	if (inside == true)
		normal *= -1;
	uv.x = dot(normalize(normal.xy), vec2(1, 0)) * 0.25 + 0.25;
	if (normal.y > 0)
		uv.x = -uv.x + 1;
	uv.y = dot(normal, vec3(0, 0, 1)) * 0.5 + 0.5;
	return (uv);
}

t_hitpoint	get_hitpoint_sphere(t_ray ray, t_sphere sphere)
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
	// hitpoint.uv = get_uv_sphere(sphere, hitpoint.normal, inside);
	hitpoint.color = sphere.base_color;
	// hitpoint.color = vec3(hitpoint.uv, 0);
	hitpoint.material_idx = sphere.material_idx;
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

