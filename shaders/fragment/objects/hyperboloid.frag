// float	get_hyperboloid_discriminant(t_ray ray, t_hyperboloid hyperboloid, out float t0, out float t1)
// {
// 	float		discriminant;
// 	float		sqrt_discriminant;

// 	vec3	v = ray.dir;					// ray dir
	
// 	vec3	va = hyperboloid.orientation;	// orientation
// 	vec3	wa = normalize(cross(va, (va != vec3(0, 0, 1)) ? vec3(0, 0, 1) : vec3(0, 1, 0)));
//   	vec3	ua = normalize(cross(va, wa));

// 	vec3	dp = ray.origin - hyperboloid.origin;	// delta p

// 	float aa = hyperboloid.a * hyperboloid.a;
// 	float bb = hyperboloid.b * hyperboloid.b;
// 	float cc = hyperboloid.c * hyperboloid.c;

// 	float dot_v_wa = dot(v, wa);
// 	float dot_dp_wa = dot(dp, wa);
// 	float dot_v_ua = dot(v, ua);
// 	float dot_dp_ua = dot(dp, ua);
// 	float dot_v_va = dot(v, va);
// 	float dot_dp_va = dot(dp, va);

// 	// version IV 
//     float A = pow(dot_v_wa, 2) / aa				+	pow(dot_v_ua, 2) / bb		-	pow(dot_v_va, 2) / cc;
//     float B = 2 * (dot_dp_wa * dot_v_wa / aa	+	dot_dp_ua * dot_v_ua / bb	-	dot_dp_va * dot_v_va / cc);
//     float C = pow(dot_dp_wa, 2) / aa			+	pow(dot_dp_ua, 2) / bb		-	pow(dot_dp_va, 2) / cc - hyperboloid.shape;

// 	discriminant = B * B - 4 * A * C;
// 	if (discriminant < 0)
// 		return (discriminant);
	
// 	sqrt_discriminant = sqrt(discriminant);
// 	if (B >= 0)
// 	{
// 		t0 = (2 * C) / (-B - sqrt_discriminant);
// 		t1 = (-B - sqrt_discriminant) / (2 * A);
// 	}
// 	else
// 	{
// 		t0 = (-B + sqrt_discriminant) / (2 * A);
// 		t1 = (2 * C) / (-B + sqrt_discriminant);
// 	}
// 	return (discriminant);
// }


t_hitpoint	get_hitpoint_hyperboloid(t_ray ray, t_hyperboloid hyperboloid)
{
	t_hitpoint	hitpoint;
	float		discriminant;
	float		sqrt_discriminant;
	float		t0;
	float		t1;
	vec3		delta_hp;	// delta_HitPoint
	float		t_height;

	// hyperboloid.orientation = vec3_rotate_x(hyperboloid.orientation, rt.debug / 50);

	// orientation vectors for each axis	
	vec3	va = hyperboloid.orientation;														// 	z-axis
	vec3	wa = normalize(cross(va, (va != vec3(0, 0, 1)) ? vec3(0, 0, 1) : vec3(0, 1, 0)));	// 	x-axis
  	vec3	ua = normalize(cross(va, wa));														// 	y-axis

	// orientation length
	float	orientation_len = dot(hyperboloid.orientation, hyperboloid.orientation);

	vec3	dp = ray.origin - hyperboloid.origin;	// delta p

	float	aa = hyperboloid.a * hyperboloid.a;
	float	bb = hyperboloid.b * hyperboloid.b;
	float	cc = hyperboloid.c * hyperboloid.c;

	vec3	v = ray.dir;					// ray dir
	float	dot_v_wa = dot(v, wa);
	float	dot_dp_wa = dot(dp, wa);
	float	dot_v_ua = dot(v, ua);
	float	dot_dp_ua = dot(dp, ua);
	float	dot_v_va = dot(v, va);
	float	dot_dp_va = dot(dp, va);

	// version IV Coefficents
    float A = pow(dot_v_wa, 2) / aa				+	pow(dot_v_ua, 2) / bb		-	pow(dot_v_va, 2) / cc;
    float B = 2 * (dot_dp_wa * dot_v_wa / aa	+	dot_dp_ua * dot_v_ua / bb	-	dot_dp_va * dot_v_va / cc);
    float C = pow(dot_dp_wa, 2) / aa			+	pow(dot_dp_ua, 2) / bb		-	pow(dot_dp_va, 2) / cc - hyperboloid.shape;

	discriminant = B * B - 4 * A * C;
	if (discriminant < 0)
		return (HP_INF);
	
	sqrt_discriminant = sqrt(discriminant);
	if (B >= 0)
	{
		t0 = (2 * C) / (-B - sqrt_discriminant);
		t1 = (-B - sqrt_discriminant) / (2 * A);
	}
	else
	{
		t0 = (-B + sqrt_discriminant) / (2 * A);
		t1 = (2 * C) / (-B + sqrt_discriminant);
	}

	hitpoint.hit = true;
	hitpoint.color = hyperboloid.base_color;
	hitpoint.material_idx = hyperboloid.material_idx;

	if (t0 > 0 && (t0 < t1 || t1 < 0))
	{
		// INSIDE
		hitpoint.ray = t0 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		delta_hp = hitpoint.pos - hyperboloid.origin;

		// height
		t_height = abs(dot(delta_hp, hyperboloid.orientation) / orientation_len);

		// check for height
		if (t_height > hyperboloid.height / 2)
		{
			// get outside HP
			hitpoint.ray = t1 * ray.dir;
			hitpoint.pos = ray.origin + hitpoint.ray;
			delta_hp = hitpoint.pos - hyperboloid.origin;

			// new height
			t_height = abs(dot(delta_hp, hyperboloid.orientation) / orientation_len);

			if (t1 < 0 || t_height > hyperboloid.height / 2)
				return (HP_INF);
			// WE ARE IN HEIGHT RANGE GET OUTSIDE HP and NORMAL
			hitpoint.normal = normalize(  dot(delta_hp, wa) * wa / aa
										+ dot(delta_hp, ua) * ua / bb
										- dot(delta_hp, va) * va / cc);
		}
		else
		{
			hitpoint.normal = -1 * normalize( dot(delta_hp, wa) * wa / aa
											+ dot(delta_hp, ua) * ua / bb
											- dot(delta_hp, va) * va / cc);
		}
	}
	else if (t1 > 0 && (t1 < t0 || t0 < 0))
	{
		// OUTSIDE
		hitpoint.ray = t1 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		delta_hp = hitpoint.pos - hyperboloid.origin;

		// height
		t_height = abs(dot(delta_hp, hyperboloid.orientation) / orientation_len);

		// check for height
		if (t_height > hyperboloid.height / 2)
		{
			// get inside HP
			hitpoint.ray = t0 * ray.dir;
			hitpoint.pos = ray.origin + hitpoint.ray;
			delta_hp = hitpoint.pos - hyperboloid.origin;

			// new height
			t_height = abs(dot(delta_hp, hyperboloid.orientation) / orientation_len);

			if (t0 < 0 || t_height > hyperboloid.height / 2)
				return (HP_INF);
			// WE ARE ABOVE HEIGHT GET INSIDE HP and NORMAL
			hitpoint.normal = -1 * normalize( dot(delta_hp, wa) * wa / aa
											+ dot(delta_hp, ua) * ua / bb
											- dot(delta_hp, va) * va / cc);
		}
		else
		{
			// WE ARE BELOW HEIGHT GET OUTSIDE HP
			hitpoint.normal = normalize(  dot(delta_hp, wa) * wa / aa
										+ dot(delta_hp, ua) * ua / bb
										- dot(delta_hp, va) * va / cc);
		}
	}
	else
		return (HP_INF);

	// hitpoint.pos += hitpoint.normal * (0.000001 * max(abs(t0), abs(t1)));	// original: 0.000001
	return (hitpoint);
}

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