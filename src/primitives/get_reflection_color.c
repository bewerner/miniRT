/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_reflection_color.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 09:44:46 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/16 16:39:51 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_vec4	shoot_ray(t_ray ray, t_rt *rt)
{
	t_hitpoint	hitpoint;
	t_vec4		col_diff;

	hitpoint = get_closest_hitpoint(ray, rt);
	if (!hitpoint.object)
		return g_vec4_black;

	col_diff = get_diffuse_color(hitpoint, rt);
	return (col_diff);
}


t_vec4	get_reflection_color(t_hitpoint hp, t_rt *rt)
{
	t_vec4	col;
	t_ray	bounce_ray;

	bounce_ray.origin = vec3_add(hp.pos, vec3_scale(0.001f, hp.normal));
	bounce_ray.dir = create_bounce_dir(hp.ray, hp.normal);
	col = shoot_ray(bounce_ray, rt);
		
	// include the base_color as meteallic materials do it like this is it mul, or add???

	return (col);
}
