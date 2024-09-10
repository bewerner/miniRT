/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 00:05:44 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/10 20:05:20 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_vec4	vec4_add(const t_vec4 a, const t_vec4 b)
{
	t_vec4	v;

	v.x = a.x + b.x;
	v.y = a.y + b.y;
	v.z = a.z + b.z;
	v.w = a.w + b.w;
	return (v);
}

t_vec4	vec4_sub(const t_vec4 a, const t_vec4 b)
{
	t_vec4	v;

	v.x = a.x - b.x;
	v.y = a.y - b.y;
	v.z = a.z - b.z;
	v.w = a.w - b.w;
	return (v);
}

t_vec4	vec4_mul(const t_vec4 a, const t_vec4 b)
{
	t_vec4	v;

	v.x = a.x * b.x;
	v.y = a.y * b.y;
	v.z = a.z * b.z;
	v.w = a.w * b.w;
	return (v);
}

t_vec4	vec4_scale(const float s, const t_vec4 a)
{
	t_vec4	v;

	v.x = a.x * s;
	v.y = a.y * s;
	v.z = a.z * s;
	v.w = a.w * s;
	return (v);
}
