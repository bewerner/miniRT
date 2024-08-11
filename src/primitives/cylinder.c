/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:16:32 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/11 21:31:13 by bwerner          ###   ########.fr       */
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
	AP = vec3_sub(AP, vec3_scale(vec3_dot(AP, object->orientation), object->orientation));
	ray.dir = vec3_sub(ray.dir, vec3_scale(vec3_dot(ray.dir, object->orientation), object->orientation));
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

t_hitpoint	get_hitpoint_cylinder(t_ray ray, t_object *object)
{
	t_object cap1_plane;
			cap1_plane.type = OBJ_PLANE;
			cap1_plane.origin = object->cap1;
			cap1_plane.normal = vec3_scale(-1, object->orientation);
			cap1_plane.base_color = object->base_color;
			cap1_plane.dist = vec3_dot(cap1_plane.origin, cap1_plane.normal);
	t_object cap2_plane;
			cap2_plane.type = OBJ_PLANE;
			cap2_plane.origin = object->cap2;
			cap2_plane.normal = object->orientation;
			cap2_plane.base_color = object->base_color;
			cap2_plane.dist = vec3_dot(cap2_plane.origin, cap2_plane.normal);
	t_hitpoint	hitpoint;
	float		t0;
	float		t1;

	if (get_discriminant(ray, object, &t0, &t1) < 0 || (t1 < 0 && t0 < 0)) // no discriminant, or both hitpoints behind me
		return (HP_INF);
	hitpoint.object = object;
	if (t1 < 0) // i am inside of the infinite cylinder
	{
		hitpoint.ray = vec3_scale(t0, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		if (vec3_dot(vec3_sub(ray.origin, object->cap2), cap2_plane.normal) > 0) // i am outside of the actual cylinder behind cap2
		{
			hitpoint = get_hitpoint_plane(ray, &cap2_plane);
			hitpoint.object = object;
			if (vec3_dist(hitpoint.pos, object->cap2) > object->radius)
				return (HP_INF);
			return (hitpoint);
		}
		else if (vec3_dot(vec3_sub(ray.origin, object->cap1), cap1_plane.normal) > 0) // i am outside of the actual cylinder behind cap1
		{
			hitpoint = get_hitpoint_plane(ray, &cap1_plane);
			hitpoint.object = object;
			if (vec3_dist(hitpoint.pos, object->cap1) > object->radius)
				return (HP_INF);
			return (hitpoint);
		}
		else if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap1)) < 0) // hitpoint is below height
		{
			
			if (vec3_dot(ray.dir, vec3_sub(object->origin, object->cap1)) < 0) // hitpoint is on cap1
				return (get_hitpoint_plane(ray, &cap1_plane));
			return (HP_INF);
		}
		else if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap2)) > 0) // hitpoint is above height
		{
			if (vec3_dot(ray.dir, vec3_sub(object->origin, object->cap2)) < 0) // hitpoint is on cap2
				return (get_hitpoint_plane(ray, &cap2_plane));
			return (HP_INF);
		}
		else // hitpoint is on cylinder
		{
			hitpoint.normal = vec3_cross(hitpoint.object->orientation, vec3_cross(vec3_sub(hitpoint.pos, hitpoint.object->origin), hitpoint.object->orientation));
			hitpoint.normal = vec3_normalize(hitpoint.normal);
			hitpoint.normal = vec3_scale(-1, hitpoint.normal);
			return (hitpoint);
		}
	}
	else // i am outside of the infinite cylinder
	{
		hitpoint.ray = vec3_scale(t1, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap1)) < 0) // hitpoint is below height
		{
			hitpoint.ray = vec3_scale(t0, ray.dir);
			hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
			if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap1)) > 0) // hitpoint is inside -> real hitpoint is on plane
				return (get_hitpoint_plane(ray, &cap1_plane));
			return (HP_INF);
		}
		else if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap2)) > 0) // hitpoint is above height
		{
			hitpoint.ray = vec3_scale(t0, ray.dir);
			hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
			if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap2)) < 0) // hitpoint is inside -> real hitpoint is on plane
				return (get_hitpoint_plane(ray, &cap2_plane));
			return (HP_INF);
		}
		else
		{
			hitpoint.normal = vec3_cross(hitpoint.object->orientation, vec3_cross(vec3_sub(hitpoint.pos, hitpoint.object->origin), hitpoint.object->orientation));
			hitpoint.normal = vec3_normalize(hitpoint.normal);
		}
		return (hitpoint);
	}
	return (hitpoint);
}

// t_hitpoint	get_hitpoint_cylinder(t_ray ray, t_object *object)
// {
// 	t_hitpoint	hitpoint;
// 	float		discriminant;
// 	float		rr;
// 	t_vec3		AP;
// 	t_vec3		tmp;
// 	t_vec3		tmp2;

// 	t_object cap1_plane;
// 	t_object cap2_plane;

// 	hitpoint.object = object;

// 	rr = object->radius * object->radius;
// 	AP = vec3_sub(ray.origin, object->origin);

// 	tmp = vec3_sub(ray.dir, vec3_scale(vec3_dot(ray.dir, object->orientation), object->orientation));
// 	float	A = vec3_dot(tmp, tmp);
// 	tmp2 = vec3_sub(AP, vec3_scale(vec3_dot(AP, object->orientation), object->orientation));
// 	float	B = 2 * vec3_dot(tmp, tmp2);
// 	float	C = vec3_dot(tmp2, tmp2) - rr;

// 	discriminant = B * B - 4 * A * C;

// 	if (discriminant < 0)
// 		return (HP_INF);

// 	float	t0 = (-B + sqrtf(discriminant)) / (2 * A);
// 	float	t1 = (-B - sqrtf(discriminant)) / (2 * A);

// 	if (t1 < 0 && t0 < 0)
// 		return (HP_INF);

// 	// create HitRay
// 	// hitpoint.pos = vec3_add(rt->camera.origin, vec3_scale(t1, rayDir));
// 	hitpoint.ray = vec3_scale(t1, ray.dir);
// 	hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
// 	if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap1)) < 0)
// 	{
// 		hitpoint.ray = vec3_scale(t0, ray.dir);
// 		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
// 		if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap1)) > 0)
// 		{	
// 			cap1_plane.type = OBJ_PLANE;
// 			cap1_plane.origin = object->cap1;
// 			cap1_plane.normal = vec3_scale(-1, object->orientation);
// 			cap1_plane.base_color = object->base_color;
// 			cap1_plane.dist = vec3_dot(cap1_plane.origin, cap1_plane.normal);
// 			hitpoint = get_hitpoint_plane(ray, &cap1_plane);
// 			hitpoint.object = object;
// 			hitpoint.normal = cap1_plane.normal;
// 			return (hitpoint);
// 		}
// 		return (HP_INF);
// 	}
// 	else if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap2)) > 0)
// 	{
// 		hitpoint.ray = vec3_scale(t0, ray.dir);
// 		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
// 		if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap2)) < 0)
// 		{	
// 			cap2_plane.type = OBJ_PLANE;
// 			cap2_plane.origin = object->cap2;
// 			cap2_plane.normal = object->orientation;
// 			cap2_plane.base_color = object->base_color;
// 			cap2_plane.dist = vec3_dot(cap2_plane.origin, cap2_plane.normal);
// 			hitpoint = get_hitpoint_plane(ray, &cap2_plane);
// 			hitpoint.object = object;
// 			hitpoint.normal = cap2_plane.normal;
// 			return (hitpoint);
// 		}
// 		return (HP_INF);
// 	}

// 	if (t1 < 0)
// 		return (HP_INF);

// 	hitpoint.normal = vec3_cross(hitpoint.object->orientation, vec3_cross(vec3_sub(hitpoint.pos, hitpoint.object->origin), hitpoint.object->orientation));
// 	hitpoint.normal = vec3_normalize(hitpoint.normal);
// 	return (hitpoint);
// }
