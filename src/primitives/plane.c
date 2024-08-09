/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:00:59 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 22:54:09 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_hitpoint	get_hitpoint_plane(t_ray ray, t_object *object)
{
	t_hitpoint	hitpoint;
	double		dp;

	dp = vec3_dot(ray.dir, object->normal);

	if (dp >= 0)
		return (HP_INF);

	double t = (object->dist - vec3_dot(ray.origin, object->normal)) / dp;

	if (t < 0)
		return (HP_INF);

	hitpoint.object = object;
	hitpoint.ray = vec3_scale(t, ray.dir);
	hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
	hitpoint.normal = object->normal;

 	return (hitpoint);
	// ray  pos, obj

}
