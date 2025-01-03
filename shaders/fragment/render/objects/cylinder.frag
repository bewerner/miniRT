void	calc_cylinder_tangent_vectors(inout t_hitpoint hitpoint, vec3 orientation, bool inside)
{
	hitpoint.tangent = cross(orientation, hitpoint.normal);
	hitpoint.bitangent = orientation;
	if (inside == true)
		hitpoint.bitangent *= -1;
}

float	get_cylinder_discriminant(t_ray ray, t_cylinder cylinder, out float t0, out float t1)
{
	vec3		ap;
	float		a;
	float		b;
	float		discriminant;
	float		sqrt_discriminant;

	ap = ray.origin - cylinder.origin;
	ap = ap - (dot(ap, cylinder.orientation) * cylinder.orientation);
	ray.dir = ray.dir - (dot(ray.dir, cylinder.orientation) * cylinder.orientation);
	a = dot(ray.dir, ray.dir);
	b = 2.0 * dot(ap, ray.dir);
	discriminant = b * b - 4 * a * (dot(ap, ap) - cylinder.radius * cylinder.radius);
	if (discriminant < 0)
		return (discriminant);
	sqrt_discriminant = sqrt(discriminant);
	t0 = (-b + sqrt_discriminant) / (2.0 * a);
	t1 = (-b - sqrt_discriminant) / (2.0 * a);
	return (discriminant);
}

t_hitpoint	get_hitpoint_cap(t_ray ray, t_cylinder cylinder, t_plane cap_plane, bool inside)
{
	t_hitpoint	hitpoint;

	if (inside)
	{
		if (dot(ray.dir, (cylinder.origin - cap_plane.origin)) < 0)
			return (get_hitpoint_plane(ray, cap_plane, false));
		return (HP_INF);
	}
	else
	{
		hitpoint = get_hitpoint_plane(ray, cap_plane, false);
		hitpoint.color = cylinder.base_color;
		hitpoint.material_idx = cylinder.material_idx;
		if (distance(hitpoint.pos, cap_plane.origin) > cylinder.radius)
			return (HP_INF);
		return (hitpoint);
	}
}

t_hitpoint	get_hitpoint_outside(t_ray ray, t_cylinder cylinder, t_hitpoint hitpoint, float t0)
{
	if (dot(cylinder.orientation, (hitpoint.pos - cylinder.cap1.origin)) < 0)
	{
		hitpoint.ray = t0 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		if (dot(cylinder.orientation, (hitpoint.pos - cylinder.cap1.origin)) > 0)
			return (get_hitpoint_plane(ray, cylinder.cap1, false));
		return (HP_INF);
	}
	else if (dot(cylinder.orientation, (hitpoint.pos - cylinder.cap2.origin)) > 0)
	{
		hitpoint.ray = t0 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		if (dot(cylinder.orientation, (hitpoint.pos - cylinder.cap2.origin)) < 0)
			return (get_hitpoint_plane(ray, cylinder.cap2, false));
		return (HP_INF);
	}
	hitpoint.normal = cross(cylinder.orientation, cross((hitpoint.pos - cylinder.origin), cylinder.orientation));
	hitpoint.normal = normalize(hitpoint.normal);
	return (hitpoint);
}

t_hitpoint	get_hitpoint_inside(t_ray ray, t_cylinder cylinder, t_hitpoint hitpoint)
{
	if (dot((ray.origin - cylinder.cap2.origin), cylinder.cap2.normal) > 0)
		return (get_hitpoint_cap(ray, cylinder, cylinder.cap2, false));
	else if (dot((ray.origin - cylinder.cap1.origin), cylinder.cap1.normal) > 0)
		return (get_hitpoint_cap(ray, cylinder, cylinder.cap1, false));
	else if (dot(cylinder.orientation, (hitpoint.pos - cylinder.cap1.origin)) < 0)
		return (get_hitpoint_cap(ray, cylinder, cylinder.cap1, true));
	else if (dot(cylinder.orientation, (hitpoint.pos - cylinder.cap2.origin)) > 0)
		return (get_hitpoint_cap(ray, cylinder, cylinder.cap2, true));
	hitpoint.normal = hitpoint.pos - cylinder.origin;
	hitpoint.normal = cross(hitpoint.normal, cylinder.orientation);
	hitpoint.normal = cross(cylinder.orientation, hitpoint.normal);
	hitpoint.normal = -1.0 * normalize(hitpoint.normal);
	return (hitpoint);
}

vec2	get_uv_cylinder(t_cylinder cylinder, vec3 orientation, vec3 normal, vec3 pos, bool inside, vec2 uv_scale)
{
	vec2	uv;

	if (inside == true)
		normal *= -1;

	uv.y = distance(cylinder.cap1.origin, pos - normal * cylinder.radius);
	uv.y = uv.y / cylinder.height / 2 + 0.5;

	if (orientation.z != 1.0)
	{
		vec2 orientation_xy = normalize(orientation.xy);
		if (orientation_xy.y != 1)
		{
			float rad = acos(dot(orientation_xy, vec2(0, 1)));
			if (orientation_xy.x < 0)
				rad *= -1;
			orientation = vec3_rotate_z(orientation, rad);
			normal = vec3_rotate_z(normal, rad);
		}
		float rad = acos(dot(normalize(orientation.yz), vec2(0, 1)));
		normal = vec3_rotate_x(normal, rad);
	}
	uv.x = 0.5 + atan(normal.x, -normal.y) / (2.0 * M_PI);
	return (uv * uv_scale);
}

vec2	get_uv_cap(vec3 cap_origin, vec3 orientation, float radius, vec3 pos, bool is_cap1, vec2 uv_scale, out vec3 tangent, out vec3 bitangent)
{
	vec2 uv;

	pos -= cap_origin;

	tangent = vec3(1.0, 0.0, 0.0);
	bitangent = vec3(0.0, 1.0, 0.0);

	if (orientation.z != 1.0)
	{
		vec2 orientation_xy = normalize(orientation.xy);
		if (orientation_xy.y != 1)
		{
			float rad = acos(dot(orientation_xy, vec2(0, 1)));
			if (orientation_xy.x < 0)
				rad *= -1;
			orientation = vec3_rotate_z(orientation, rad);
			pos = vec3_rotate_z(pos, rad);
			tangent = vec3_rotate_z(tangent, -rad);
			bitangent = vec3_rotate_z(bitangent, -rad);
		}
		float rad = acos(dot(normalize(orientation.yz), vec2(0, 1)));
		pos = vec3_rotate_x(pos, rad);
		tangent = vec3_rotate_x(tangent, rad);
		bitangent = vec3_rotate_x(bitangent, rad);
	}
	tangent = normalize(tangent);
	bitangent = normalize(bitangent);

	uv = pos.xy / radius;
	uv *= 0.24;
	uv += 0.25;
	if (is_cap1 == true)
		uv.x += 0.5;

	return (uv * uv_scale);
}

t_hitpoint	get_hitpoint_cylinder(t_ray ray, t_cylinder cylinder, bool init_all)
{
	t_hitpoint	hitpoint;
	float		t0;
	float		t1;

	hitpoint.hit = true;
	if (get_cylinder_discriminant(ray, cylinder, t0, t1) < 0 || (t1 < 0 && t0 < 0))
		return (HP_INF);
	if (t1 < 0)
	{
		hitpoint.ray = t0 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		hitpoint = get_hitpoint_inside(ray, cylinder, hitpoint);
	}
	else
	{
		hitpoint.ray = t1 * ray.dir;
		hitpoint.pos = ray.origin + hitpoint.ray;
		hitpoint = get_hitpoint_outside(ray, cylinder, hitpoint, t0);
	}
	hitpoint.color = cylinder.base_color;
	hitpoint.material_idx = cylinder.material_idx;
	hitpoint.object_normal = hitpoint.normal;

	if (init_all == false)
		return (hitpoint);

	bool inside =  t1 < 0
				&& dot(cylinder.cap1.origin - ray.origin, cylinder.cap1.normal) > 0
				&& dot(cylinder.cap2.origin - ray.origin, cylinder.cap2.normal) > 0;
	bool is_cap1 = (inside == false && hitpoint.normal == cylinder.cap1.normal)
				|| (inside == true && hitpoint.normal == -cylinder.cap1.normal);
	bool is_cap2 = (inside == false && hitpoint.normal == cylinder.cap2.normal)
				|| (inside == true && hitpoint.normal == -cylinder.cap2.normal);


	// if we have an image_texture we calculate UVs
	if (has_image_texture(hitpoint))
	{
		if (is_cap1 == true)
			hitpoint.uv = get_uv_cap(cylinder.cap1.origin, cylinder.orientation, cylinder.radius, hitpoint.pos, true, cylinder.uv_scale, hitpoint.tangent, hitpoint.bitangent);
		else if (is_cap2 == true)
			hitpoint.uv = get_uv_cap(cylinder.cap2.origin, cylinder.orientation, cylinder.radius, hitpoint.pos, false, cylinder.uv_scale, hitpoint.tangent, hitpoint.bitangent);
		else
			hitpoint.uv = get_uv_cylinder(cylinder, cylinder.orientation, hitpoint.normal, hitpoint.pos, inside, cylinder.uv_scale);
	}

	// We need tangent and bitangent vectors for normal_maps		
	if (has_normal_map_material(hitpoint))
	{
		// get_uv_cap already sets tangents
		if (is_cap1 == false && is_cap2 == false)
			calc_cylinder_tangent_vectors(hitpoint, cylinder.orientation, inside);
		hitpoint.normal = apply_normal_map(hitpoint);
	}
	return (hitpoint);
}

t_cylinder	get_cylinder(int offset)
{
	t_cylinder	cylinder;

	cylinder.type = int(texelFetch(objects, offset++).r);
	cylinder.next_offset = int(texelFetch(objects, offset++).r);
	cylinder.origin = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	cylinder.base_color = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	cylinder.orientation = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	cylinder.radius = texelFetch(objects, offset++).r;
	cylinder.height = texelFetch(objects, offset++).r;
	cylinder.uv_scale = vec2(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	cylinder.material_idx = int(texelFetch(objects, offset++).r);
	cylinder.cap1 = get_plane(offset);
	offset = cylinder.cap1.next_offset;
	cylinder.cap2 = get_plane(offset);

	return (cylinder);
}
