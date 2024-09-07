t_hitpoint	get_hitpoint_sphere(t_ray ray, t_sphere sphere)
{
	t_hitpoint	hitpoint;
	float		discriminant;
	float		t0;
	float		t1;

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
	return (hitpoint);
}

t_sphere	get_sphere(int offset)
{
	t_sphere	sphere;

	sphere.type = int(texelFetch(objects, offset++).r);
	sphere.next_offset = int(texelFetch(objects, offset++).r);
	sphere.origin = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	sphere.base_color = vec4(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	sphere.radius = texelFetch(objects, offset++).r;
	sphere.material_idx = int(texelFetch(objects, offset).r);

	return (sphere);
}

