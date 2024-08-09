/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:30:03 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 17:14:49 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_hitpoint	get_hitpoint_sphere(t_ray ray, t_object *object)
{
	t_hitpoint	hitpoint;
	double		discriminant;
	double		rr;
	t_vec3		AP;

	rr = object->radius * object->radius;
	AP = vec3_sub(ray.origin, object->origin);

	double	A = vec3_dot(ray.dir, ray.dir);
	double	B = 2 * vec3_dot(AP, ray.dir);
	double	C = vec3_dot(AP, AP) - rr;

	discriminant = B * B - 4 * A * C;

	if (discriminant < 0)
		return (HP_INF);

	// double	t0 = (-B  sqrt(discriminant)) / (2 * A)		// DO WE NEED THIS LATER ?? 
	double	t1 = (-B - sqrt(discriminant)) / (2 * A);

	if (t1 < 0)
		return (HP_INF);

	// create HitRay
	// hitpoint.pos = vec3_add(rt->camera.origin, vec3_scale(t1, rayDir));
	hitpoint.object = object;
	hitpoint.ray = vec3_scale(t1, ray.dir);
	hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
	hitpoint.normal = vec3_normalize(vec3_sub(hitpoint.pos, object->origin));

	return (hitpoint);
}

bool	is_obstructed(t_ray ray, t_object *exclude, t_rt *rt)
{
	size_t		i;
	t_hitpoint	current;
	double		ray_len;

	current.ray = VEC3_INF;
	current.pos = VEC3_INF;
	current.object = NULL;
	ray_len = vec3_len(ray.dir);
	i = 0;
	while (rt->objects[i].type)
	{
		if (&rt->objects[i] == exclude)
		{
			i++;
			continue ;
		}
		if (rt->objects[i].type == OBJ_SPHERE )
			current = get_hitpoint_sphere(ray, &rt->objects[i]);
		else if (rt->objects[i].type == OBJ_PLANE)
			current = get_hitpoint_plane(ray, &rt->objects[i]);
		else if (rt->objects[i].type == OBJ_CYLINDER)
			current = get_hitpoint_cylinder(ray, &rt->objects[i]);
		if (vec3_len(current.ray) < ray_len)
			return (true);
		i++;
	}
	return (false);
}
