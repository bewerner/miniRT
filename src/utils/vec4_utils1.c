/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec4_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 00:05:44 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/04 21:10:51 by nmihaile         ###   ########.fr       */
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

t_vec4	vec4_scale(const double s, const t_vec4 a)
{
	t_vec4	v;

	v.x = a.x * s;
	v.y = a.y * s;
	v.z = a.z * s;
	v.w = a.w * s;
	return (v);
}
