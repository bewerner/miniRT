/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:20:22 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

float	vec3_len(const t_vec3 a)
{
	float	dot;

	dot = vec3_dot(a, a);
	if (dot == INFINITY || dot == -INFINITY)
		return (dot);
	return (sqrtf(dot));
}

float	vec3_dist(const t_vec3 a, const t_vec3 b)
{
	return (vec3_len(vec3_sub(a, b)));
}

t_vec3	vec3_normalize(const t_vec3 a)
{
	float	len;
	t_vec3	v;

	len = vec3_len(a);
	if (len == 0.0f)
		return ((t_vec3){{0, 0, 0}});
	v.x = a.x / len;
	v.y = a.y / len;
	v.z = a.z / len;
	return (v);
}

t_vec3	vec3_cross(const t_vec3 a, const t_vec3 b)
{
	t_vec3	v;

	v.x = -a.z * b.y + a.y * b.z;
	v.y = a.z * b.x - a.x * b.z;
	v.z = -a.y * b.x + a.x * b.y;
	return (v);
}
