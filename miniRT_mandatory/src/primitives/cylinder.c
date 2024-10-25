/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:16:32 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/14 22:51:04 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static float	get_discriminant(
		t_ray ray, t_cylinder *cylinder, float *t0, float *t1)
{
	t_vec3		ap;
	float		a;
	float		b;
	float		discriminant;
	float		sqrt_discriminant;

	ap = vec3_sub(ray.origin, cylinder->origin);
	ap = vec3_sub(ap, vec3_scale(
				vec3_dot(ap, cylinder->orientation), cylinder->orientation));
	ray.dir = vec3_sub(ray.dir, vec3_scale(vec3_dot(
					ray.dir, cylinder->orientation), cylinder->orientation));
	a = vec3_dot(ray.dir, ray.dir);
	b = 2 * vec3_dot(ap, ray.dir);
	discriminant = b * b - 4 * a * (
			vec3_dot(ap, ap) - cylinder->radius * cylinder->radius);
	if (discriminant < 0)
		return (discriminant);
	sqrt_discriminant = sqrtf(discriminant);
	*t0 = (-b + sqrt_discriminant) / (2 * a);
	*t1 = (-b - sqrt_discriminant) / (2 * a);
	return (discriminant);
}

static t_hitpoint	get_hitpoint_cap(
	t_ray ray, t_cylinder *cylinder, t_plane *cap, bool inside)
{
	t_hitpoint	hitpoint;

	if (inside)
	{
		if (vec3_dot(ray.dir, vec3_sub(cylinder->origin, cap->origin)) < 0)
			return (get_hitpoint_plane(ray, cap));
		return (g_hp_inf);
	}
	else
	{
		hitpoint = get_hitpoint_plane(ray, cap);
		if (vec3_dist(hitpoint.pos, cap->origin) > cylinder->radius)
			return (g_hp_inf);
		return (hitpoint);
	}
}

static t_hitpoint	get_hitpoint_outside(
	t_ray ray, t_cylinder *cylinder, t_hitpoint hitpoint, float t0)
{
	if (vec3_dot(cylinder->orientation, vec3_sub(
				hitpoint.pos, cylinder->cap1.origin)) < 0)
	{
		hitpoint.ray = vec3_scale(t0, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		if (vec3_dot(cylinder->orientation, vec3_sub(
					hitpoint.pos, cylinder->cap1.origin)) > 0)
			return (get_hitpoint_plane(ray, &cylinder->cap1));
		return (g_hp_inf);
	}
	else if (vec3_dot(cylinder->orientation, vec3_sub(
				hitpoint.pos, cylinder->cap2.origin)) > 0)
	{
		hitpoint.ray = vec3_scale(t0, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		if (vec3_dot(cylinder->orientation, vec3_sub(
					hitpoint.pos, cylinder->cap2.origin)) < 0)
			return (get_hitpoint_plane(ray, &cylinder->cap2));
		return (g_hp_inf);
	}
	hitpoint.normal = vec3_cross(cylinder->orientation, vec3_cross(vec3_sub(
					hitpoint.pos, cylinder->origin), cylinder->orientation));
	hitpoint.normal = vec3_normalize(hitpoint.normal);
	return (hitpoint);
}

static t_hitpoint	get_hitpoint_inside(
		t_ray ray, t_cylinder *cylinder, t_hitpoint hitpoint)
{
	if (vec3_dot(vec3_sub(ray.origin, cylinder->cap2.origin),
			cylinder->cap2.normal) > 0)
		return (get_hitpoint_cap(ray, cylinder, &cylinder->cap2, false));
	else if (vec3_dot(vec3_sub(ray.origin, cylinder->cap1.origin),
			cylinder->cap1.normal) > 0)
		return (get_hitpoint_cap(ray, cylinder, &cylinder->cap1, false));
	else if (vec3_dot(cylinder->orientation, vec3_sub(hitpoint.pos,
				cylinder->cap1.origin)) < 0)
		return (get_hitpoint_cap(ray, cylinder, &cylinder->cap1, true));
	else if (vec3_dot(cylinder->orientation, vec3_sub(hitpoint.pos,
				cylinder->cap2.origin)) > 0)
		return (get_hitpoint_cap(ray, cylinder, &cylinder->cap2, true));
	hitpoint.normal = vec3_sub(hitpoint.pos, cylinder->origin);
	hitpoint.normal = vec3_cross(hitpoint.normal, cylinder->orientation);
	hitpoint.normal = vec3_cross(cylinder->orientation, hitpoint.normal);
	hitpoint.normal = vec3_scale(-1, vec3_normalize(hitpoint.normal));
	return (hitpoint);
}

t_hitpoint	get_hitpoint_cylinder(t_ray ray, t_cylinder *cylinder)
{
	t_hitpoint	hitpoint;
	float		t0;
	float		t1;

	if (get_discriminant(ray, cylinder, &t0, &t1) < 0 || (t1 < 0 && t0 < 0))
		return (g_hp_inf);
	if (t1 < 0)
	{
		hitpoint.ray = vec3_scale(t0, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		return (get_hitpoint_inside(ray, cylinder, hitpoint));
	}
	else
	{
		hitpoint.ray = vec3_scale(t1, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		return (get_hitpoint_outside(ray, cylinder, hitpoint, t0));
	}
}
