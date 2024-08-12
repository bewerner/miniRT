/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:16:32 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/12 19:16:35 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static float	get_discriminant(t_ray ray, t_cylinder *cylinder, float *t0, float *t1)
{
	t_vec3		AP;
	float		A;
	float		B;
	float		discriminant;
	float		sqrt_discriminant;

	AP = vec3_sub(ray.origin, cylinder->origin);
	AP = vec3_sub(AP, vec3_scale(vec3_dot(AP, cylinder->orientation), cylinder->orientation));
	ray.dir = vec3_sub(ray.dir, vec3_scale(vec3_dot(ray.dir, cylinder->orientation), cylinder->orientation));
	A = vec3_dot(ray.dir, ray.dir);
	B = 2 * vec3_dot(AP, ray.dir);
	discriminant = B * B - 4 * A * (vec3_dot(AP, AP) - cylinder->radius * cylinder->radius);
	if (discriminant < 0)
		return (discriminant);
	sqrt_discriminant = sqrtf(discriminant);
	*t0 = (-B + sqrt_discriminant) / (2 * A);
	*t1 = (-B - sqrt_discriminant) / (2 * A);
	return (discriminant);
}

t_hitpoint	get_hitpoint_cylinder(t_ray ray, t_cylinder *cylinder)
{
	t_plane cap1_plane;
			cap1_plane.type = OBJ_PLANE;
			cap1_plane.origin = cylinder->cap1;
			cap1_plane.normal = vec3_scale(-1, cylinder->orientation);
			cap1_plane.base_color = cylinder->base_color;
			cap1_plane.dist = vec3_dot(cap1_plane.origin, cap1_plane.normal);
	t_plane cap2_plane;
			cap2_plane.type = OBJ_PLANE;
			cap2_plane.origin = cylinder->cap2;
			cap2_plane.normal = cylinder->orientation;
			cap2_plane.base_color = cylinder->base_color;
			cap2_plane.dist = vec3_dot(cap2_plane.origin, cap2_plane.normal);
	t_hitpoint	hitpoint;
	float		t0;
	float		t1;

	if (get_discriminant(ray, cylinder, &t0, &t1) < 0 || (t1 < 0 && t0 < 0)) // no discriminant, or both hitpoints behind me
		return (HP_INF);
	if (t1 < 0) // i am inside of the infinite cylinder
	{
		hitpoint.ray = vec3_scale(t0, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		if (vec3_dot(vec3_sub(ray.origin, cylinder->cap2), cap2_plane.normal) > 0) // i am outside of the actual cylinder behind cap2
		{
			hitpoint = get_hitpoint_plane(ray, &cap2_plane);
			if (vec3_dist(hitpoint.pos, cylinder->cap2) > cylinder->radius)
				return (HP_INF);
			return (hitpoint);
		}
		else if (vec3_dot(vec3_sub(ray.origin, cylinder->cap1), cap1_plane.normal) > 0) // i am outside of the actual cylinder behind cap1
		{
			hitpoint = get_hitpoint_plane(ray, &cap1_plane);
			if (vec3_dist(hitpoint.pos, cylinder->cap1) > cylinder->radius)
				return (HP_INF);
			return (hitpoint);
		}
		else if (vec3_dot(cylinder->orientation, vec3_sub(hitpoint.pos, cylinder->cap1)) < 0) // hitpoint is below height
		{
			
			if (vec3_dot(ray.dir, vec3_sub(cylinder->origin, cylinder->cap1)) < 0) // hitpoint is on cap1
				return (get_hitpoint_plane(ray, &cap1_plane));
			return (HP_INF);
		}
		else if (vec3_dot(cylinder->orientation, vec3_sub(hitpoint.pos, cylinder->cap2)) > 0) // hitpoint is above height
		{
			if (vec3_dot(ray.dir, vec3_sub(cylinder->origin, cylinder->cap2)) < 0) // hitpoint is on cap2
				return (get_hitpoint_plane(ray, &cap2_plane));
			return (HP_INF);
		}
		else // hitpoint is on cylinder
		{
			hitpoint.normal = vec3_cross(cylinder->orientation, vec3_cross(vec3_sub(hitpoint.pos, cylinder->origin), cylinder->orientation));
			hitpoint.normal = vec3_normalize(hitpoint.normal);
			hitpoint.normal = vec3_scale(-1, hitpoint.normal);
			return (hitpoint);
		}
	}
	else // i am outside of the infinite cylinder
	{
		hitpoint.ray = vec3_scale(t1, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		if (vec3_dot(cylinder->orientation, vec3_sub(hitpoint.pos, cylinder->cap1)) < 0) // hitpoint is below height
		{
			hitpoint.ray = vec3_scale(t0, ray.dir);
			hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
			if (vec3_dot(cylinder->orientation, vec3_sub(hitpoint.pos, cylinder->cap1)) > 0) // hitpoint is inside -> real hitpoint is on plane
				return (get_hitpoint_plane(ray, &cap1_plane));
			return (HP_INF);
		}
		else if (vec3_dot(cylinder->orientation, vec3_sub(hitpoint.pos, cylinder->cap2)) > 0) // hitpoint is above height
		{
			hitpoint.ray = vec3_scale(t0, ray.dir);
			hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
			if (vec3_dot(cylinder->orientation, vec3_sub(hitpoint.pos, cylinder->cap2)) < 0) // hitpoint is inside -> real hitpoint is on plane
				return (get_hitpoint_plane(ray, &cap2_plane));
			return (HP_INF);
		}
		else
		{
			hitpoint.normal = vec3_cross(cylinder->orientation, vec3_cross(vec3_sub(hitpoint.pos, cylinder->origin), cylinder->orientation));
			hitpoint.normal = vec3_normalize(hitpoint.normal);
		}
		return (hitpoint);
	}
	return (hitpoint);
}
