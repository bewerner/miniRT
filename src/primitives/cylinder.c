/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 17:16:32 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/09 15:43:30 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_hitpoint	get_hitpoint_cylinder(t_ray ray, t_object *object)
{
	t_hitpoint	hitpoint;
	double		discriminant;
	double		rr;
	t_vec3		AP;
	t_vec3		tmp;
	t_vec3		tmp2;

	t_object cap1_plane;
	t_object cap2_plane;

	hitpoint.object = object;

	rr = object->radius * object->radius;
	AP = vec3_sub(ray.origin, object->origin);

	tmp = vec3_sub(ray.dir, vec3_scale(vec3_dot(ray.dir, object->orientation), object->orientation));
	double	A = vec3_dot(tmp, tmp);
	tmp2 = vec3_sub(AP, vec3_scale(vec3_dot(AP, object->orientation), object->orientation));
	double	B = 2 * vec3_dot(tmp, tmp2);
	double	C = vec3_dot(tmp2, tmp2) - rr;

	discriminant = B * B - 4 * A * C;

	if (discriminant < 0)
		return (HP_INF);

	double	t0 = (-B + sqrt(discriminant)) / (2 * A);
	double	t1 = (-B - sqrt(discriminant)) / (2 * A);

	if (t1 < 0 && t0 < 0)
		return (HP_INF);

	// create HitRay
	// hitpoint.pos = vec3_add(rt->camera.origin, vec3_scale(t1, rayDir));
	hitpoint.ray = vec3_scale(t1, ray.dir);
	hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
	if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap1)) < 0)
	{
		hitpoint.ray = vec3_scale(t0, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap1)) > 0)
		{	
			cap1_plane.type = OBJ_PLANE;
			cap1_plane.origin = object->cap1;
			cap1_plane.normal = vec3_scale(-1, object->orientation);
			cap1_plane.base_color = object->base_color;
			cap1_plane.dist = vec3_dot(cap1_plane.origin, cap1_plane.normal);
			hitpoint = get_hitpoint_plane(ray, &cap1_plane);
			hitpoint.object = object;
			hitpoint.normal = cap1_plane.normal;
			return (hitpoint);
		}
		return (HP_INF);
	}
	else if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap2)) > 0)
	{
		hitpoint.ray = vec3_scale(t0, ray.dir);
		hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
		if (vec3_dot(object->orientation, vec3_sub(hitpoint.pos, object->cap2)) < 0)
		{	
			cap2_plane.type = OBJ_PLANE;
			cap2_plane.origin = object->cap2;
			cap2_plane.normal = object->orientation;
			cap2_plane.base_color = object->base_color;
			cap2_plane.dist = vec3_dot(cap2_plane.origin, cap2_plane.normal);
			hitpoint = get_hitpoint_plane(ray, &cap2_plane);
			hitpoint.object = object;
			hitpoint.normal = cap2_plane.normal;
			return (hitpoint);
		}
		return (HP_INF);
	}

	hitpoint.normal = vec3_cross(hitpoint.object->orientation, vec3_cross(vec3_sub(hitpoint.pos, hitpoint.object->origin), hitpoint.object->orientation));
	return (hitpoint);
}

t_vec4	get_diffuse_color_cylinder(t_hitpoint hitpoint, t_rt *rt)
{
	t_vec4	col;
	t_vec3	normal;
	// t_vec3	tmp;
	t_ray	light_ray;
	double	distance;
	int		i;
	t_vec4	combined_light_col;
	double	received_intensity;

	// REFACTOR INTO get_hitpoint_OBJ function // also for plane !!!
	hitpoint.pos = vec3_add(rt->camera.origin, hitpoint.ray); // change HitRay to HitPoint

	// tmp = vec3_add(hitpoint.object->origin, hitpoint.object->orientation);
	// normal = vec3_cross(tmp, vec3_cross(hitpoint.pos, tmp));

	normal = hitpoint.normal;


	
	combined_light_col = VEC4_BLACK;

	i = -1;
	while (rt->lights[++i].type)
	{
		light_ray.dir = vec3_sub(hitpoint.pos, rt->lights[i].origin);
		light_ray.origin = rt->lights[i].origin;
		received_intensity = -vec3_dot(normal, vec3_normalize(light_ray.dir));
		if (received_intensity <= 0)
			continue ;
		distance = vec3_len(light_ray.dir);
		received_intensity = received_intensity / (distance * distance); // Apply Falloff
		if (received_intensity <= 0.0f) // 1 / 256 = ~0.0039
			continue ;
		if (is_obstructed(light_ray, hitpoint.object, rt)) // Check for Ligth Collision
			continue ;
		received_intensity *= rt->lights[i].intensity;
		col = (t_vec4){{rt->lights[i].color.r * received_intensity, rt->lights[i].color.g * received_intensity, rt->lights[i].color.b * received_intensity, 1.0f}};
		combined_light_col = vec4_add(combined_light_col, col);
	}
	combined_light_col.a = 1;
	combined_light_col = vec4_add(combined_light_col, rt->ambient);

	// combine Light color with base color
	combined_light_col = vec4_mul(hitpoint.object->base_color, combined_light_col);

	return (combined_light_col);
}