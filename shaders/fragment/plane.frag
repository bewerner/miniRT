
bool	is_near_zero(float value)
{
	return (value > -EPSILON && value < EPSILON);
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
	hitpoint.color = plane.base_color;
	return (hitpoint);
}

t_plane	get_plane(int offset)
{
	t_plane	plane;

	plane.type = int(texelFetch(objects, offset++).r);
	plane.next_offset = int(texelFetch(objects, offset++).r);
	plane.origin = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	plane.base_color = vec4(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	plane.normal = vec3(texelFetch(objects, offset++).r, texelFetch(objects, offset++).r, texelFetch(objects, offset++).r);
	plane.dist = int(texelFetch(objects, offset++).r);

	return (plane);
}

