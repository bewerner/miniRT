/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:00:59 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/10 18:07:21 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static bool	is_near_zero(double value)
{
	return (value > -0.0001 && value < 0.0001);
}

t_hitpoint	get_hitpoint_plane(t_ray ray, t_object *object)
{
	t_hitpoint	hitpoint;
	double		dp;

	if (is_near_zero(vec3_dot(vec3_sub(object->origin, ray.origin), object->normal)))
		return (HP_INF);
	dp = vec3_dot(ray.dir, object->normal);
	if (is_near_zero(dp))
		return (HP_INF);
	if (dp > 0)
		hitpoint.normal = vec3_scale(-1, object->normal);
	else
		hitpoint.normal = object->normal;
//	else if (backface culling is on)
//		return (HP_INF);
	double t = (object->dist - vec3_dot(ray.origin, object->normal)) / dp;
	if (t < 0)
		return (HP_INF);
	hitpoint.object = object;
	hitpoint.ray = vec3_scale(t, ray.dir);
	hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
 	return (hitpoint);
}
