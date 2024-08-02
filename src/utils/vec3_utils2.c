/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 16:20:22 by nmihaile          #+#    #+#             */
/*   Updated: 2024/07/31 17:14:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

double	vec3_len(const t_vec3 a)
{
	return (sqrt(vec3_dot(a, a)));
}

t_vec3	vec3_normalize(const t_vec3 a)
{
	double	len;
	t_vec3	v;

	len = vec3_len(a);
	if (len == 0.0f)
		return ((t_vec3){0, 0, 0});
	v.x = a.x / len;
	v.y = a.y / len;
	v.z = a.z / len;
	return (v);
}
