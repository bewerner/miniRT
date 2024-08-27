float	get_sphere_discriminant(t_ray ray, t_sphere sphere, out float t0, out float t1)
{
	vec3		ap;
	float		a;
	float		b;
	float		discriminant;
	float		sqrt_discriminant;

	ap = ray.origin - sphere.origin;
	a = dot(ray.dir, ray.dir);
	b = 2 * dot(ap, ray.dir);
	discriminant = b * b - 4 * a * (dot(ap, ap) - sphere.radius * sphere.radius);
	if (discriminant < 0)
		return (discriminant);
	sqrt_discriminant = sqrt(discriminant);
	t0 = (-b + sqrt_discriminant) / (2 * a);
	t1 = (-b - sqrt_discriminant) / (2 * a);
	return (discriminant);
}

t_hitpoint	get_hitpoint_sphere(t_ray ray, t_sphere sphere)
{
	t_hitpoint	hitpoint;
	float		discriminant;
	float		t0;
	float		t1;

	discriminant = get_sphere_discriminant(ray, sphere, t0, t1);
	if (discriminant < 0 || (t1 < 0 && t0 < 0))
		return (t_hitpoint(false, VEC3_INF, VEC3_INF, VEC3_INF, VEC4_BLACK));
	if (t1 < 0)
	{
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
	return (hitpoint);
}

t_sphere	get_sphere(int offset)
{
	t_sphere	sphere;

	// sphere.type = int(texelFetch(objects, offset).r);
	// sphere.next_offset = int(texelFetch(objects, offset + 1).r);
	// sphere.origin = vec3(texelFetch(objects, offset + 2).r, texelFetch(objects, offset + 3).r, texelFetch(objects, offset + 4).r);
	// sphere.base_color = vec4(texelFetch(objects, offset + 5).r, texelFetch(objects, offset + 6).r, texelFetch(objects, offset + 7).r, texelFetch(objects, offset + 8).r);
	// sphere.radius = texelFetch(objects, offset + 9).r;

	sphere.type = int(texelFetch(objects, offset++).r);
	sphere.next_offset = int(texelFetch(objects, offset++).r);
	sphere.origin = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	sphere.base_color = vec4(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	sphere.radius = texelFetch(objects, offset++).r;

	return (sphere);
}

