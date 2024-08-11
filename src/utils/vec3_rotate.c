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

t_vec3	vec3_rotate_x(t_vec3 v, float rad)
{
	float	s;
	float	c;

	s = sinf(rad);
	c = cosf(rad);
	return ((t_vec3){v.x, v.y * c - v.z * s, v.y * s + v.z * c});
}

t_vec3	vec3_rotate_y(t_vec3 v, float rad)
{
	float	s;
	float	c;

	s = sinf(rad);
	c = cosf(rad);
	return ((t_vec3){v.x * c + v.z * s, v.y, -v.x * s + v.z * c});
}

t_vec3	vec3_rotate_z(t_vec3 v, float rad)
{
	float	s;
	float	c;

	s = sinf(rad);
	c = cosf(rad);
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

// t_vec3	vec3_rotate_x(t_vec3 p, float rad)
// {
// 	t_vec3	v;
// 	v = p;
// 	v = vec3_transform_m3(v,
// 		(t_vec3){1, 0, 0},
// 		(t_vec3){0, cosf(rad), -sinf(rad)},
// 		(t_vec3){0, sinf(rad), cosf(rad)});
// 	// v = vec3_transform_m3(v,
// 	// 	(t_vec3){cosf(rad), 0, sinf(rad)},
// 	// 	(t_vec3){1, 0, 0},
// 	// 	(t_vec3){-sinf(rad), 0, cosf(rad)});
// 	// v = vec3_transform_m3(v,
// 	// 	(t_vec3){cosf(yaw), -sinf(yaw), 0},
// 	// 	(t_vec3){sinf(yaw), cosf(yaw), 0},
// 	// 	(t_vec3){0, 0, 1});

// 	return (v);
// }

// t_vec3	vec3_rotate_y(t_vec3 p, float rad)
// {
// 	t_vec3	v;
// 	v = p;
// 	// v = vec3_transform_m3(v,
// 	// 	(t_vec3){1, 0, 0},
// 	// 	(t_vec3){0, cosf(pitch), -sinf(pitch)},
// 	// 	(t_vec3){0, sinf(pitch), cosf(pitch)});
// 	v = vec3_transform_m3(v,
// 		(t_vec3){cosf(rad), 0, sinf(rad)},
// 		(t_vec3){1, 0, 0},
// 		(t_vec3){-sinf(rad), 0, cosf(rad)});
// 	// v = vec3_transform_m3(v,
// 	// 	(t_vec3){cosf(yaw), -sinf(yaw), 0},
// 	// 	(t_vec3){sinf(yaw), cosf(yaw), 0},
// 	// 	(t_vec3){0, 0, 1});

// 	return (v);
// }

// t_vec3	vec3_rotate_z(t_vec3 p, float rad)
// {
// 	t_vec3	v;
// 	v = p;
// 	// v = vec3_transform_m3(v,
// 	// 	(t_vec3){1, 0, 0},
// 	// 	(t_vec3){0, cosf(pitch), -sinf(pitch)},
// 	// 	(t_vec3){0, sinf(pitch), cosf(pitch)});
// 	// v = vec3_transform_m3(v,
// 	// 	(t_vec3){cosf(pitch), 0, sinf(pitch)},
// 	// 	(t_vec3){1, 0, 0},
// 	// 	(t_vec3){-sinf(pitch), 0, cosf(pitch)});
// 	v = vec3_transform_m3(v,
// 		(t_vec3){cosf(rad), -sinf(rad), 0},
// 		(t_vec3){sinf(rad), cosf(rad), 0},
// 		(t_vec3){0, 0, 1});

// 	return (v);
// }