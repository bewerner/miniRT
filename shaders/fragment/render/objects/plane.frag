bool	is_near_zero(float value)
{
	return (value > -EPSILON && value < EPSILON);
}

vec2	get_uv_plane(vec3 normal, vec3 plane_origin, vec3 pos, vec2 uv_scale, out vec3 tangent, out vec3 bitangent)
{
	vec2	uv;

	tangent = vec3(1.0, 0.0, 0.0);
	bitangent = vec3(0.0, 1.0, 0.0);

	if (abs(normal.z) != 1)
	{
		vec2 normal_xy = normalize(normal.xy);
		if (normal_xy.y != -1)
		{
			float rad = acos(dot(normal_xy, vec2(0, -1)));
			if (normal_xy.x > 0)
				rad *= -1;
			pos = vec3_rotate_z(pos, rad);
			normal = vec3_rotate_z(normal, rad);
			tangent = vec3_rotate_z(tangent, -rad);
			bitangent = vec3_rotate_z(bitangent, -rad);
		}
		float rad = acos(dot(normalize(normal.yz), vec2(0, 1)));
		pos = vec3_rotate_x(pos, -rad);
		tangent = vec3_rotate_x(tangent, -rad);
		bitangent = vec3_rotate_x(bitangent, -rad);
	}
	tangent = normalize(tangent);
	bitangent = normalize(bitangent);

	// uv = pos.xy - plane_origin.xy;
	uv = pos.xy - plane_origin.xy - rt.debug * vec2(0.038, 0.052);

	return (uv * uv_scale / 2);
}

t_hitpoint	get_hitpoint_plane(t_ray ray, t_plane plane, bool init_all)
{
	t_hitpoint	hitpoint;
	float		dp;
	float		t;

	if (is_near_zero(dot(plane.origin - ray.origin, plane.normal)))
		return (HP_INF);
	dp = dot(ray.dir, plane.normal);
	if (is_near_zero(dp))
		return (HP_INF);
	if (dp > 0)
		hitpoint.normal = -1 * plane.normal;
	else
		hitpoint.normal = plane.normal;
	t = (plane.dist - dot(ray.origin, plane.normal)) / dp;
	if (t < 0)
		return (HP_INF);
	hitpoint.hit = true;
	hitpoint.ray = t * ray.dir;
	hitpoint.pos = ray.origin + hitpoint.ray;
	hitpoint.color = plane.base_color;
	hitpoint.material_idx = plane.material_idx;
	hitpoint.object_normal = hitpoint.normal;

	if (init_all == false)
		return (hitpoint);

	// if we have an image_texture we calculate UVs
	if (has_image_texture(hitpoint))
		hitpoint.uv = get_uv_plane(plane.normal, plane.origin, hitpoint.pos, plane.uv_scale, hitpoint.tangent, hitpoint.bitangent);

	if (has_normal_map_material(hitpoint))
		hitpoint.normal = apply_normal_map(hitpoint);

	return (hitpoint);
}

t_plane	get_plane(int offset)
{
	t_plane	plane;

	plane.type = int(texelFetch(objects, offset++).r);
	plane.next_offset = int(texelFetch(objects, offset++).r);
	plane.origin = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	plane.base_color = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	plane.normal = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	plane.dist = texelFetch(objects, offset++).r;
	plane.uv_scale = vec2(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	plane.material_idx = int(texelFetch(objects, offset).r);

	return (plane);
}
