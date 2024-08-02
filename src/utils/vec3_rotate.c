/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_rotation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 14:47:52 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/02 18:00:21 by nmihaile         ###   ########.fr       */
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

// static t_vec3	vec3_transform_m3(t_vec3 p, t_vec3 m0, t_vec3 m1, t_vec3 m2)
// {
// 	t_vec3	v;
// 	v.x = vec3_dot(p, m0);
// 	v.y = vec3_dot(p, m1);
// 	v.z = vec3_dot(p, m2);

// 	return (v);
// }

// t_vec3	vec3_rotate_x(t_vec3 p, double rad)
// {
// 	t_vec3	v;
// 	v = p;
// 	v = vec3_transform_m3(v,
// 		(t_vec3){1, 0, 0},
// 		(t_vec3){0, cos(rad), -sin(rad)},
// 		(t_vec3){0, sin(rad), cos(rad)});
// 	// v = vec3_transform_m3(v,
// 	// 	(t_vec3){cos(rad), 0, sin(rad)},
// 	// 	(t_vec3){1, 0, 0},
// 	// 	(t_vec3){-sin(rad), 0, cos(rad)});
// 	// v = vec3_transform_m3(v,
// 	// 	(t_vec3){cos(yaw), -sin(yaw), 0},
// 	// 	(t_vec3){sin(yaw), cos(yaw), 0},
// 	// 	(t_vec3){0, 0, 1});

// 	return (v);
// }

// t_vec3	vec3_rotate_y(t_vec3 p, double rad)
// {
// 	t_vec3	v;
// 	v = p;
// 	// v = vec3_transform_m3(v,
// 	// 	(t_vec3){1, 0, 0},
// 	// 	(t_vec3){0, cos(pitch), -sin(pitch)},
// 	// 	(t_vec3){0, sin(pitch), cos(pitch)});
// 	v = vec3_transform_m3(v,
// 		(t_vec3){cos(rad), 0, sin(rad)},
// 		(t_vec3){1, 0, 0},
// 		(t_vec3){-sin(rad), 0, cos(rad)});
// 	// v = vec3_transform_m3(v,
// 	// 	(t_vec3){cos(yaw), -sin(yaw), 0},
// 	// 	(t_vec3){sin(yaw), cos(yaw), 0},
// 	// 	(t_vec3){0, 0, 1});

// 	return (v);
// }

// t_vec3	vec3_rotate_z(t_vec3 p, double rad)
// {
// 	t_vec3	v;
// 	v = p;
// 	// v = vec3_transform_m3(v,
// 	// 	(t_vec3){1, 0, 0},
// 	// 	(t_vec3){0, cos(pitch), -sin(pitch)},
// 	// 	(t_vec3){0, sin(pitch), cos(pitch)});
// 	// v = vec3_transform_m3(v,
// 	// 	(t_vec3){cos(pitch), 0, sin(pitch)},
// 	// 	(t_vec3){1, 0, 0},
// 	// 	(t_vec3){-sin(pitch), 0, cos(pitch)});
// 	v = vec3_transform_m3(v,
// 		(t_vec3){cos(rad), -sin(rad), 0},
// 		(t_vec3){sin(rad), cos(rad), 0},
// 		(t_vec3){0, 0, 1});

// 	return (v);
// }