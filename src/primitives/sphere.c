/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:30:03 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 18:25:20 by bwerner          ###   ########.fr       */
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
