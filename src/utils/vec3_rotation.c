/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_rotation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:47:52 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

t_vec3	vec3_rotate_x(t_vec3 v, float rad)
{
	float	s;
	float	c;

	s = sinf(rad);
	c = cosf(rad);
	return ((t_vec3){{v.x, v.y * c - v.z * s, v.y * s + v.z * c}});
}

t_vec3	vec3_rotate_y(t_vec3 v, float rad)
{
	float	s;
	float	c;

	s = sinf(rad);
	c = cosf(rad);
	return ((t_vec3){{v.x * c + v.z * s, v.y, -v.x * s + v.z * c}});
}

t_vec3	vec3_rotate_z(t_vec3 v, float rad)
{
	float	s;
	float	c;

	s = sinf(rad);
	c = cosf(rad);
	return ((t_vec3){{v.x * c - v.y * s, v.x * s + v.y * c, v.z}});
}
