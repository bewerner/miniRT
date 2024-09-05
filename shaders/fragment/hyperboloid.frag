
float	get_hyperboloid_discriminant(t_ray ray, t_hyperboloid hyperboloid, out float t0, out float t1)
{
	vec3		ap;		// origin - point_offset
	float		a;
	float		b;
	float		c;
	vec3		d;		// denominator
	vec3		ad;		// ray: a + bt -> a * d
	vec3		bd;		// ray: a + bt -> b * d
	vec3		doth;	// hyperboloid dot_product -> where (dot = x*x + y*y - z*z)
	float		discriminant;
	float		sqrt_discriminant;

	// setup denominator(coefficients) and the other variables
	d = vec3(1 / hyperboloid.a, 1 / hyperboloid.b, 1 / hyperboloid.c);
	ap = ray.origin - hyperboloid.origin;
	ad = ap * d;
	bd = ray.dir * d;
	doth = vec3(1, 1, -1);

	a = dot(doth * bd, bd);
	b = 2 * dot(doth * ad, bd);
	c = dot(doth * ad, ad) - hyperboloid.shape;
	discriminant = b * b - 4 * a * c;
	if (discriminant < 0)
		return (discriminant);
	sqrt_discriminant = sqrt(discriminant);
	t0 = (-b + sqrt_discriminant) / (2 * a);
	t1 = (-b - sqrt_discriminant) / (2 * a);
	return (discriminant);
}

float	in_or_out(t_ray ray, t_hyperboloid hyperboloid)
{
	return ((ray.origin.x * ray.origin.x) / (hyperboloid.a * hyperboloid.a) +
			(ray.origin.y * ray.origin.y) / (hyperboloid.b * hyperboloid.b) -
			(ray.origin.z * ray.origin.z) / (hyperboloid.c * hyperboloid.c));
}

t_hitpoint	get_hitpoint_hyperboloid(t_ray ray, t_hyperboloid hyperboloid)
{
	t_hitpoint	hitpoint;
	float		discriminant;
	float		t0;
	float		t1;

	float dbg = rt.debug;
	if (dbg == 0)
		dbg = EPSILON;

	hyperboloid.a = dbg / 100;
	hyperboloid.b = dbg / 100;
	

	discriminant = get_hyperboloid_discriminant(ray, hyperboloid, t0, t1);

	if (discriminant < 0 || (t1 < 0 && t0 < 0))
		// 1. If (t0 < 0) and (t1 < 0) : then the ray does not intersect the hyperboloid.
		return (HP_INF);

	if (t0 - t1 <= EPSILON)
	{
		// tangent ray
		hitpoint.ray = 1 / EPSILON * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		hitpoint.normal = normalize(vec3(	-2 * (hitpoint.pos.x - hyperboloid.origin.x) / hyperboloid.a * hyperboloid.a,
											-2 * (hitpoint.pos.y - hyperboloid.origin.y) / hyperboloid.b * hyperboloid.b,
											 2 * (hitpoint.pos.z / hyperboloid.c  - hyperboloid.origin.z)* hyperboloid.c));
		hitpoint.normal = vec3(0,1,0);
	}
	if (t0 < 0 && t1 > 0)
	{	// 2. If (t0 < 0) and (t1 > 0) : then the ray intersects the hyperboloid at a single point, which is located at a distance t1 along the ray.
		hitpoint.ray = t1 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		hitpoint.normal = normalize(vec3(	 2 * (hitpoint.pos.x - hyperboloid.origin.x) / hyperboloid.a * hyperboloid.a,
											 2 * (hitpoint.pos.y - hyperboloid.origin.y) / hyperboloid.b * hyperboloid.b,
											-2 * (hitpoint.pos.z - hyperboloid.origin.z) / hyperboloid.c * hyperboloid.c));
	}
	else if (t0 > 0 && t1 > 0)
	{	// 3. If (t0 > 0) and (t1 > 0) : then the ray intersects the hyperboloid at two points, which are located at distances t0 and t1 along the ray.
		float outside = in_or_out(ray, hyperboloid);
		if (outside > 0.0)
		{	// OUTSIDE
			if (t0 - t1 < EPSILON)
			{
				hitpoint.ray = t0 * ray.dir;
				hitpoint.pos = ray.origin + hitpoint.ray;
				hitpoint.normal = normalize(vec3(	-2 * (hitpoint.pos.x - hyperboloid.origin.x) / hyperboloid.a * hyperboloid.a,
													-2 * (hitpoint.pos.y - hyperboloid.origin.y) / hyperboloid.b * hyperboloid.b,
													 2 * (hitpoint.pos.z - hyperboloid.origin.z) / hyperboloid.c * hyperboloid.c));
			}
			else
			{
				hitpoint.ray = t1 * ray.dir;
				hitpoint.pos = ray.origin + hitpoint.ray;
				hitpoint.normal = normalize(vec3(	 2 * (hitpoint.pos.x - hyperboloid.origin.x) / hyperboloid.a * hyperboloid.a,
													 2 * (hitpoint.pos.y - hyperboloid.origin.y) / hyperboloid.b * hyperboloid.b,
													-2 * (hitpoint.pos.z - hyperboloid.origin.z) / hyperboloid.c * hyperboloid.c));
			}
		}
		else
		{	// INSIDE
			hitpoint.ray = t0 * ray.dir;
			hitpoint.pos = ray.origin + hitpoint.ray;
			hitpoint.normal = normalize(vec3(	-2 * (hitpoint.pos.x - hyperboloid.origin.x) / hyperboloid.a * hyperboloid.a,
												-2 * (hitpoint.pos.y - hyperboloid.origin.y) / hyperboloid.b * hyperboloid.b,
												 2 * (hitpoint.pos.z - hyperboloid.origin.z) / hyperboloid.c * hyperboloid.c));
		}
	}
	else if (t1 < 0)
	{
		// INSIDE
		hitpoint.ray = t0 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		hitpoint.normal = normalize(vec3(	-2 * (hitpoint.pos.x - hyperboloid.origin.x) / hyperboloid.a * hyperboloid.a,
											-2 * (hitpoint.pos.y - hyperboloid.origin.y) / hyperboloid.b * hyperboloid.b,
											 2 * (hitpoint.pos.z / hyperboloid.c  - hyperboloid.origin.z)* hyperboloid.c));
	}
	hitpoint.hit = true;
	hitpoint.color = hyperboloid.base_color;
	hitpoint.material_idx = hyperboloid.material_idx;
	return (hitpoint);
}

// 4. If (t0 > 0) and (t1 < 0) : then this is an indication of an error in the calculation, and it should be investigated further.
// 5. If (t0 == t1) then the ray is tangent to the hyperboloid at a single point, which is located at a distance t0 (or t1) along the ray.

t_hyperboloid	get_hyperboloid(int offset)
{
	t_hyperboloid	hyperboloid;

	hyperboloid.type = int(texelFetch(objects, offset++).r);
	hyperboloid.next_offset = int(texelFetch(objects, offset++).r);
	hyperboloid.origin = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	hyperboloid.base_color = vec4(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	hyperboloid.a = texelFetch(objects, offset++).r;
	hyperboloid.b = texelFetch(objects, offset++).r;
	hyperboloid.c = texelFetch(objects, offset++).r;
	hyperboloid.shape = texelFetch(objects, offset++).r;
	hyperboloid.material_idx = int(texelFetch(objects, offset++).r);

	return (hyperboloid);
}
