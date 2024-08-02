/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_rotation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:47:52 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/02 22:25:18 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_vec3	vec3_rotate_x(t_vec3 v, double rad)
{
	double	s;
	double	c;

	s = sin(rad);
	c = cos(rad);
	return ((t_vec3){v.x, v.y * c - v.z * s, v.y * s + v.z * c});
}

t_vec3	vec3_rotate_y(t_vec3 v, double rad)
{
	double	s;
	double	c;

	s = sin(rad);
	c = cos(rad);
	return ((t_vec3){v.x * c + v.z * s, v.y, -v.x * s + v.z * c});
}

t_vec3	vec3_rotate_z(t_vec3 v, double rad)
{
	double	s;
	double	c;

	s = sin(rad);
	c = cos(rad);
	return ((t_vec3){v.x * c - v.y * s, v.x * s + v.y * c, v.z});
}
