/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 12:56:40 by nmihaile          #+#    #+#             */
/*   Updated: 2024/07/31 15:55:35 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_vec3	vec3_add(const t_vec3 a, const t_vec3 b)
{
	t_vec3	v;

	v.x = a.x + b.x;
	v.x = a.y + b.y;
	v.x = a.z + b.z;
	return (v);
}

t_vec3	vec3_sub(const t_vec3 a, const t_vec3 b)
{
	t_vec3	v;

	v.x = a.x - b.x;
	v.x = a.y - b.y;
	v.x = a.z - b.z;
	return (v);
}

t_vec3	vec3_mul(const t_vec3 a, const t_vec3 b)
{
	t_vec3	v;

	v.x = a.x * b.x;
	v.y = a.y * b.y;
	v.z = a.z * b.z;
	return (v);
}

t_vec3	vec3_scale(const double s, const t_vec3 a)
{
	t_vec3	v;

	v.x = a.x * s;
	v.y = a.y * s;
	v.z = a.z * s;
	return (v);
}

double	vec3_dot(const t_vec3 a, const t_vec3 b)
{
	return (a.x * b.x + a.y * b.y + a.z * b.z);
}
