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
	// rotate ap
	// ap = ap - (dot(ap, hyperboloid.orientation) * hyperboloid.orientation);
	// ray.dir = ray.dir - (dot(ray.dir, hyperboloid.orientation) * hyperboloid.orientation);

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

// bool is_on_surface(vec3 point, t_hyperboloid hyperboloid)
// {
// 	float x = point.x - hyperboloid.origin.x;
// 	float y = point.y - hyperboloid.origin.y;
// 	float z = point.z - hyperboloid.origin.z;

// 	float left = x * x / (hyperboloid.a * hyperboloid.a) + y * y / (hyperboloid.b * hyperboloid.b) - z * z / (hyperboloid.c * hyperboloid.c);
// 	float right = hyperboloid.shape;

// 	return ( abs(left - right) < EPSILON );
// }


t_hitpoint	get_hitpoint_hyperboloid(t_ray ray, t_hyperboloid hyperboloid)
{
	t_hitpoint	hitpoint;
	float		discriminant;
	float		t0;
	float		t1;

	discriminant = get_hyperboloid_discriminant(ray, hyperboloid, t0, t1);

	if (discriminant < EPSILON)
		return (HP_INF);

	hitpoint.hit = true;
	hitpoint.color = hyperboloid.base_color;
	hitpoint.material_idx = hyperboloid.material_idx;

	if (t0 > 0 && (t0 < t1 || t1 < 0))
	{
		// INSIDE
		hitpoint.ray = t0 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;

		// ap = ap - (dot(ap, hyperboloid.orientation) * hyperboloid.orientation);
		// vec3	rot_height = hitpoint.pos - (dot(hitpoint.pos, hyperboloid.orientation) * hyperboloid.orientation);

		// check for height
		if (abs(hitpoint.pos.z) > hyperboloid.height)
		{
			// get outside HP
			hitpoint.ray = t1 * ray.dir;
			hitpoint.pos = ray.origin + hitpoint.ray;

			if (t1 < 0 || abs(hitpoint.pos.z) > hyperboloid.height)
				return (HP_INF);
			// WE ARE IN HEIGHT RANGE GET OUTSIDE HP and NORMAL
			hitpoint.normal = normalize(vec3
			(
				 2 * (hitpoint.pos.x - hyperboloid.origin.x) / (hyperboloid.a * hyperboloid.a),
				 2 * (hitpoint.pos.y - hyperboloid.origin.y) / (hyperboloid.b * hyperboloid.b),
				-2 * (hitpoint.pos.z - hyperboloid.origin.z) / (hyperboloid.c * hyperboloid.c)
			));
		}
		else
		{
			hitpoint.normal = normalize(vec3
			(
				-2 * (hitpoint.pos.x - hyperboloid.origin.x) / (hyperboloid.a * hyperboloid.a),
				-2 * (hitpoint.pos.y - hyperboloid.origin.y) / (hyperboloid.b * hyperboloid.b),
				 2 * (hitpoint.pos.z - hyperboloid.origin.z) / (hyperboloid.c * hyperboloid.c)
			));
		}

		// rotate normal
		// hitpoint.normal = hitpoint.normal - (dot(hitpoint.normal, hyperboloid.orientation) * hyperboloid.orientation);

	}
	else if (t1 > 0 && (t1 < t0 || t0 < 0))
	{
		// OUTSIDE
		hitpoint.ray = t1 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		
		// check for height
		if (abs(hitpoint.pos.z) > hyperboloid.height)
		{
			// get inside HP
			hitpoint.ray = t0 * ray.dir;
			hitpoint.pos = ray.origin + hitpoint.ray;
			if (t0 < 0 || abs(hitpoint.pos.z) > hyperboloid.height)
				return (HP_INF);
			// WE ARE ABOVE HEIGHT GET INSIDE HP and NORMAL
			hitpoint.normal = normalize(vec3
			(
				-2 * (hitpoint.pos.x - hyperboloid.origin.x) / (hyperboloid.a * hyperboloid.a),
				-2 * (hitpoint.pos.y - hyperboloid.origin.y) / (hyperboloid.b * hyperboloid.b),
				 2 * (hitpoint.pos.z - hyperboloid.origin.z) / (hyperboloid.c * hyperboloid.c)
			));
		}
		else
		{
			// WE ARE BELOW HEIGHT GET OUTSIDE HP
			hitpoint.normal = normalize(vec3
			(
				 2 * (hitpoint.pos.x - hyperboloid.origin.x) / (hyperboloid.a * hyperboloid.a),
				 2 * (hitpoint.pos.y - hyperboloid.origin.y) / (hyperboloid.b * hyperboloid.b),
				-2 * (hitpoint.pos.z - hyperboloid.origin.z) / (hyperboloid.c * hyperboloid.c)
			));
		}
	}
	else
		return (HP_INF);

	// hitpoint.pos += hitpoint.normal * (0.000001 * max(abs(t0), abs(t1)));
	return (hitpoint);
}



///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


// t_hitpoint	get_hitpoint_hyperboloid(t_ray ray, t_hyperboloid hyperboloid)
// {
// 	t_hitpoint	hitpoint;
// 	float		discriminant;
// 	float		t0;
// 	float		t1;

// 	discriminant = get_hyperboloid_discriminant(ray, hyperboloid, t0, t1);

// 	if (discriminant < 0)
// 		return (HP_INF);

// 	hitpoint.hit = true;
// 	hitpoint.color = hyperboloid.base_color;
// 	hitpoint.material_idx = hyperboloid.material_idx;

// 	if (t0 > 0 && (t0 < t1 || t1 < 0))
// 	{
// 		// INSIDE
// 		hitpoint.ray = t0 * ray.dir;
// 		hitpoint.pos = ray.origin + hitpoint.ray;
// 		hitpoint.normal = normalize(vec3
// 		(
// 			-2 * (hitpoint.pos.x - hyperboloid.origin.x) / (hyperboloid.a * hyperboloid.a),
// 			-2 * (hitpoint.pos.y - hyperboloid.origin.y) / (hyperboloid.b * hyperboloid.b),
// 			 2 * (hitpoint.pos.z - hyperboloid.origin.z) / (hyperboloid.c * hyperboloid.c)
// 		));
// 		// if (t1 > 0)
// 		// {
// 		// 	// INSIDE INF_DISC
// 		// 	hitpoint.ray = t0 * ray.dir;
// 		// 	hitpoint.pos = ray.origin + hitpoint.ray;
// 		// 	hitpoint.normal = normalize(vec3
// 		// 	(
// 		// 		-2 * (hitpoint.pos.x - hyperboloid.origin.x) / (hyperboloid.a * hyperboloid.a),
// 		// 		-2 * (hitpoint.pos.y - hyperboloid.origin.y) / (hyperboloid.b * hyperboloid.b),
// 		// 		 2 * (hitpoint.pos.z - hyperboloid.origin.z) / (hyperboloid.c * hyperboloid.c)
// 		// 	));
// 		// }
// 		// else
// 		// {
// 		// 	// INSIDE CONE_PART
// 		// 	hitpoint.ray = t0 * ray.dir;
// 		// 	hitpoint.pos = ray.origin + hitpoint.ray;
// 		// 	hitpoint.normal = normalize(vec3
// 		// 	(
// 		// 		-2 * (hitpoint.pos.x - hyperboloid.origin.x) / (hyperboloid.a * hyperboloid.a),
// 		// 		-2 * (hitpoint.pos.y - hyperboloid.origin.y) / (hyperboloid.b * hyperboloid.b),
// 		// 		 2 * (hitpoint.pos.z - hyperboloid.origin.z) / (hyperboloid.c * hyperboloid.c)
// 		// 	));
// 		// }
// 	}
// 	else if (t1 > 0 && (t1 < t0 || t0 < 0))
// 	{
// 		// OUTSIDE
// 		hitpoint.ray = t1 * ray.dir;
// 		hitpoint.pos = ray.origin + hitpoint.ray;
// 		hitpoint.normal = normalize(vec3
// 		(
// 			 2 * (hitpoint.pos.x - hyperboloid.origin.x) / (hyperboloid.a * hyperboloid.a),
// 			 2 * (hitpoint.pos.y - hyperboloid.origin.y) / (hyperboloid.b * hyperboloid.b),
// 			-2 * (hitpoint.pos.z - hyperboloid.origin.z) / (hyperboloid.c * hyperboloid.c)
// 		));
// 		// if (t0 > 0)
// 		// {
// 		// 	// OUTSIDE CONE_PART
// 		// 	hitpoint.ray = t1 * ray.dir;
// 		// 	hitpoint.pos = ray.origin + hitpoint.ray;
// 		// 	hitpoint.normal = normalize(vec3
// 		// 	(
// 		// 		 2 * (hitpoint.pos.x - hyperboloid.origin.x) / (hyperboloid.a * hyperboloid.a),
// 		// 		 2 * (hitpoint.pos.y - hyperboloid.origin.y) / (hyperboloid.b * hyperboloid.b),
// 		// 		-2 * (hitpoint.pos.z - hyperboloid.origin.z) / (hyperboloid.c * hyperboloid.c)
// 		// 	));
// 		// }
// 		// else
// 		// {
// 		// 	// OUTSIDE INF_DISC
// 		// 	hitpoint.ray = t1 * ray.dir;
// 		// 	hitpoint.pos = ray.origin + hitpoint.ray;
// 		// 	hitpoint.normal = normalize(vec3
// 		// 	(
// 		// 		 2 * (hitpoint.pos.x - hyperboloid.origin.x) / (hyperboloid.a * hyperboloid.a),
// 		// 		 2 * (hitpoint.pos.y - hyperboloid.origin.y) / (hyperboloid.b * hyperboloid.b),
// 		// 		-2 * (hitpoint.pos.z - hyperboloid.origin.z) / (hyperboloid.c * hyperboloid.c)
// 		// 	));
// 		// }
// 	}
// 	else
// 		return (HP_INF);

// 	// check if we are on the surface
// 	// if (is_on_surface(rt.camera.origin, hyperboloid) == true)
// 	// {
// 	// 	hitpoint.ray = vec3(0, 0, 0);
// 	// 	hitpoint.pos = rt.camera.origin;
// 	// 	hitpoint.normal = normalize(vec3
// 	// 	(
// 	// 			2 * (hitpoint.pos.x - hyperboloid.origin.x) / (hyperboloid.a * hyperboloid.a),
// 	// 			2 * (hitpoint.pos.y - hyperboloid.origin.y) / (hyperboloid.b * hyperboloid.b),
// 	// 		-2 * (hitpoint.pos.z - hyperboloid.origin.z) / (hyperboloid.c * hyperboloid.c)
// 	// 	));
// 	// }


// 	// check for height
// 	// if (abs(hitpoint.pos.z) > hyperboloid.height)
// 	// 	return (HP_INF);


// 	// hitpoint.pos += hitpoint.normal * 0.02;
// 	// hitpoint.ray -= hitpoint.normal * 0.02;
// 	return (hitpoint);
// }


///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


t_hyperboloid	get_hyperboloid(int offset)
{
	t_hyperboloid	hyperboloid;

	hyperboloid.type = int(texelFetch(objects, offset++).r);
	hyperboloid.next_offset = int(texelFetch(objects, offset++).r);
	hyperboloid.origin = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	hyperboloid.base_color = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	hyperboloid.orientation = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	hyperboloid.height = texelFetch(objects, offset++).r;
	hyperboloid.a = texelFetch(objects, offset++).r;
	hyperboloid.b = texelFetch(objects, offset++).r;
	hyperboloid.c = texelFetch(objects, offset++).r;
	hyperboloid.shape = texelFetch(objects, offset++).r;
	hyperboloid.material_idx = int(texelFetch(objects, offset++).r);

	return (hyperboloid);
}
