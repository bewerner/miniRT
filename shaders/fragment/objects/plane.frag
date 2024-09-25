bool	is_near_zero(float value)
{
	return (value > -EPSILON && value < EPSILON);
}

vec2	get_uv_plane(t_plane plane, vec3 pos)
{
	vec2	uv;
	float	scale = 5;

	if (plane.normal.z == 1.0)
		plane.normal.y = -0.000001;
	if (plane.normal.z == -1.0)
		plane.normal.y = 0.000001;

	vec3 norm_u = normalize(cross(plane.normal, vec3(plane.normal.xy, 0)));
	vec3 norm_v = cross(plane.normal, norm_u);

	norm_u /= scale;
	norm_v /= scale;

	vec3 relative_pos = pos - plane.origin;
	uv.x = dot(relative_pos, norm_u);
	uv.y = dot(relative_pos, norm_v);

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
	hitpoint.uv = get_uv_plane(plane, hitpoint.pos);
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
