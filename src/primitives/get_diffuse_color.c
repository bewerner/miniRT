/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_diffuse_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 18:23:37 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/09 18:25:03 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_vec4	get_diffuse_color(t_hitpoint hitpoint, t_rt *rt)
{
	int		i;
	t_ray	light_ray;
	double	distance;
	t_vec4	col;
	double	intensity;

	col = VEC4_BLACK;
	i = -1;
	while (rt->lights[++i].type)
	{
		light_ray.dir = vec3_sub(hitpoint.pos, rt->lights[i].origin);
		light_ray.origin = rt->lights[i].origin;
		intensity = -vec3_dot(hitpoint.normal, vec3_normalize(light_ray.dir));
		if (intensity <= 0)
			continue ;
		distance = vec3_len(light_ray.dir);
		intensity = intensity / (distance * distance);
		if (intensity <= 0.0f || is_obstructed(light_ray, hitpoint.object, rt))
			continue ;
		intensity *= rt->lights[i].intensity;
		col = vec4_add(col, vec4_scale(intensity, rt->lights[i].color));
	}
	col = vec4_add(col, rt->ambient);
	col = vec4_mul(hitpoint.object->base_color, col);
	return (col);
}
