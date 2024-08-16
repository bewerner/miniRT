/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_specular_color.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:57:12 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/16 18:55:36 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static t_vec4	illuminate_from_point_light(
	t_point_light *point_light, t_hitpoint *hitpoint, t_rt *rt)
{
	t_ray	light_ray;
	float	distance;
	float	intensity;

	light_ray.dir = vec3_sub(vec3_add(hitpoint->pos,
				vec3_scale(0.001, hitpoint->normal)), point_light->origin);
	light_ray.origin = point_light->origin;
	intensity = -vec3_dot(hitpoint->normal, vec3_normalize(light_ray.dir));
	if (intensity <= 0)
		return (g_vec4_black);
	distance = vec3_len(light_ray.dir);
	intensity = intensity / (distance * distance);
	if (intensity <= 0.000005 || is_obstructed(light_ray, rt))
		return (g_vec4_black);
	intensity *= point_light->intensity;
	return (vec4_scale(intensity, point_light->color));
}

t_vec4	get_specular_color(t_hitpoint hitpoint, t_rt *rt)
{
	t_vec4	col;
	t_light	*light;
	float	intensity;
	t_vec3	bounce_dir;
	t_vec3	light_dir;

	col = g_vec4_black;
	light = rt->lights;
	while (light)
	{
		if (light->type == LIGHT_POINT)
		{
			bounce_dir = vec3_normalize(create_bounce_dir(hitpoint.ray, hitpoint.normal));
			light_dir = vec3_normalize(vec3_sub(light->origin, hitpoint.pos));
			intensity = vec3_dot(bounce_dir, light_dir);
			if (intensity > 0)
			{
				intensity = 1 * powf(intensity, 10); // multiply with specular value (0.0 - 1.0) and to the power of some roughness calculation
				col = vec4_add(col, vec4_scale(intensity, illuminate_from_point_light((t_point_light *)light, &hitpoint, rt)));
			}
		}
		light = light->next;
	}
	return (col);
}
