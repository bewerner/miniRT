bool	is_near_zero(float value)
{
	return (value > -EPSILON && value < EPSILON);
}

vec2	get_uv_plane(vec3 normal, vec3 plane_origin, vec3 pos)
{
	vec2	uv;
	float	scale = 5;

	if (rt.debug == 0 && abs(normal.z) != 1.0)
	{
		vec3 axis = normalize(cross(normal, vec3(0, 0, 1)));
		float rad = acos(dot(normal, vec3(0, 0, 1)));
		pos = vec3_rotate_axis(pos, axis, rad);
		if (axis.x != 1.0)
		{
			rad = acos(dot(vec3(1, 0, 0), axis));
			pos = vec3_rotate_z(pos, rad);
		}
		pos = -pos;
	}

	uv = pos.xy - plane_origin.xy;

	uv = fract(uv);
	return (uv);
}

t_hitpoint	get_hitpoint_plane(t_ray ray, t_plane plane)
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
	hitpoint.uv = get_uv_plane(plane.normal, plane.origin, hitpoint.pos);
	hitpoint.color = plane.base_color;
	// hitpoint.color = vec3(hitpoint.uv, 0);
	hitpoint.material_idx = plane.material_idx;

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
	plane.material_idx = int(texelFetch(objects, offset).r);

	return (plane);
}
