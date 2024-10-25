vec3	vec3_rotate_x(vec3 v, float rad)
{
	float	s;
	float	c;

	s = sin(rad);
	c = cos(rad);
	return (vec3(v.x, v.y * c - v.z * s, v.y * s + v.z * c));
}

vec3	vec3_rotate_y(vec3 v, float rad)
{
	float	s;
	float	c;

	s = sin(rad);
	c = cos(rad);
	return (vec3(v.x * c + v.z * s, v.y, -v.x * s + v.z * c));
}

vec3	vec3_rotate_z(vec3 v, float rad)
{
	float	s;
	float	c;

	s = sin(rad);
	c = cos(rad);
	return (vec3(v.x * c - v.y * s, v.x * s + v.y * c, v.z));
}

vec3	vec3_rotate_axis(vec3 v, vec3 axis, float rad)
{
	float s = sin(rad);
	float c = cos(rad);
	axis = normalize(axis);
	// Rotate vector v around axis by rad using Rodrigues' rotation formula
	return (v * c + cross(axis, v) * s + axis * dot(axis, v) * (1 - c));
}
