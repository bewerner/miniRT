/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:30:03 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/11 10:43:22 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static float	get_discriminant(t_ray ray, t_object *object, float *t0, float *t1)
{
	t_vec3		AP;
	float		A;
	float		B;
	float		discriminant;
	float		sqrt_discriminant;

	AP = vec3_sub(ray.origin, object->origin);
	A = vec3_dot(ray.dir, ray.dir);
	B = 2 * vec3_dot(AP, ray.dir);
	discriminant = B * B - 4 * A * (vec3_dot(AP, AP) - object->radius * object->radius);
	if (discriminant < 0)
		return (discriminant);
	sqrt_discriminant = sqrtf(discriminant);
	*t0 = (-B + sqrt_discriminant) / (2 * A);
	*t1 = (-B - sqrt_discriminant) / (2 * A);
	return (discriminant);
}

t_hitpoint	get_hitpoint_sphere(t_ray ray, t_object *object)
{
	t_hitpoint	hitpoint;
	float		discriminant;
	float		t0;
	float		t1;

	discriminant = get_discriminant(ray, object, &t0, &t1);
	if (discriminant < 0 || (t1 < 0 && t0 < 0))
	 	return (HP_INF);
	if (t1 < 0)
	{
		hitpoint.ray = vec3_scale(t0, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		hitpoint.normal = vec3_normalize(vec3_sub(object->origin, hitpoint.pos));
	}
	else
	{
		hitpoint.ray = vec3_scale(t1, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		hitpoint.normal = vec3_normalize(vec3_sub(hitpoint.pos, object->origin));
	}
	hitpoint.object = object;
	return (hitpoint);
}
