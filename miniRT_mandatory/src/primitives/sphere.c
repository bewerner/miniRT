/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:30:03 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/14 22:45:51 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static float	get_discriminant(
		t_ray ray, t_sphere *sphere, float *t0, float *t1)
{
	t_vec3		ap;
	float		a;
	float		b;
	float		discriminant;
	float		sqrt_discriminant;

	ap = vec3_sub(ray.origin, sphere->origin);
	a = vec3_dot(ray.dir, ray.dir);
	b = 2 * vec3_dot(ap, ray.dir);
	discriminant = b * b - 4 * a * (vec3_dot(ap, ap)
			- sphere->radius * sphere->radius);
	if (discriminant < 0)
		return (discriminant);
	sqrt_discriminant = sqrtf(discriminant);
	*t0 = (-b + sqrt_discriminant) / (2 * a);
	*t1 = (-b - sqrt_discriminant) / (2 * a);
	return (discriminant);
}

t_hitpoint	get_hitpoint_sphere(t_ray ray, t_sphere *sphere)
{
	t_hitpoint	hitpoint;
	float		discriminant;
	float		t0;
	float		t1;

	discriminant = get_discriminant(ray, sphere, &t0, &t1);
	if (discriminant < 0 || (t1 < 0 && t0 < 0))
		return (g_hp_inf);
	if (t1 < 0)
	{
		hitpoint.ray = vec3_scale(t0, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		hitpoint.normal
			= vec3_normalize(vec3_sub(sphere->origin, hitpoint.pos));
	}
	else
	{
		hitpoint.ray = vec3_scale(t1, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		hitpoint.normal
			= vec3_normalize(vec3_sub(hitpoint.pos, sphere->origin));
	}
	hitpoint.object = (t_object *)sphere;
	return (hitpoint);
}
