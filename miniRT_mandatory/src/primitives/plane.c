/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:00:59 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/14 22:45:43 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static bool	is_near_zero(float value)
{
	return (value > -0.0001 && value < 0.0001);
}

t_hitpoint	get_hitpoint_plane(t_ray ray, t_plane *plane)
{
	t_hitpoint	hitpoint;
	float		dp;
	float		t;

	if (is_near_zero(
			vec3_dot(vec3_sub(plane->origin, ray.origin), plane->normal)))
		return (g_hp_inf);
	dp = vec3_dot(ray.dir, plane->normal);
	if (is_near_zero(dp))
		return (g_hp_inf);
	if (dp > 0)
		hitpoint.normal = vec3_scale(-1, plane->normal);
	else
		hitpoint.normal = plane->normal;
	t = (plane->dist - vec3_dot(ray.origin, plane->normal)) / dp;
	if (t < 0)
		return (g_hp_inf);
	hitpoint.object = (t_object *)plane;
	hitpoint.ray = vec3_scale(t, ray.dir);
	hitpoint.pos = vec3_add(ray.origin, hitpoint.ray);
	return (hitpoint);
}
