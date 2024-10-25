float	get_light_gizmo_discriminant(t_ray ray, vec3 point)
{
	vec3		ap;
	float		a;
	float		b;
	float		discriminant;
	float		sqrt_discriminant;
	float 		radius = 0.15;

	ap = ray.origin - point;
	a = dot(ray.dir, ray.dir);
	b = 2 * dot(ap, ray.dir);
	discriminant = b * b - 4 * a * (dot(ap, ap) - radius * radius);
	if (discriminant < 0)
		return (discriminant);
	sqrt_discriminant = sqrt(discriminant);
	float t1 = (-b - sqrt_discriminant) / (2 * a);
	if (t1 > 0)
		return (t1);
	return (0.0);
}

bool	is_ligth_gizmo(t_ray ray)
{
	t_point_light	light;

	int i = -1;
	int type = next_light_type(i);
	while (type != LIGHT_NONE)
	{
		if (type == LIGHT_POINT)
		{
			light = get_point_light(i);

			float t1 = get_light_gizmo_discriminant(ray, light.origin);

			// Use Fresnel as outline
			if (t1 > 0 ) // && t1 < 0.1
			{
				vec3 hp_ray = t1 * ray.dir;
				vec3 pos = ray.origin + hp_ray;
				vec3 normal = normalize(pos - light.origin);

				float fresnel_intensity = 1.0;
				float fresnel = (abs(dot(ray.dir, normal)) * 0.5 + 0.5) * fresnel_intensity;
				if (fresnel < 0.8 || fresnel >= 0.99 )
					return (true);
			}

		}
		type = next_light_type(i);
	}
	return (false);
}
