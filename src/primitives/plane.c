/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 16:00:59 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 15:43:38 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_hitpoint	get_hitpoint_plane(t_ray ray, t_object *object)
{
	t_hitpoint	hitpoint;
	double		dp;

	dp = vec3_dot(ray.dir, object->normal);

	if (dp >= 0)
		return (HP_INF);

	double t = (object->dist - vec3_dot(ray.origin, object->normal)) / dp;

	if (t < 0)
		return (HP_INF);

	hitpoint.ray = vec3_scale(t, ray.dir);
	hitpoint.object = object;

 	return (hitpoint);
	// ray  pos, obj

}

t_vec4	get_diffuse_color_plane(t_hitpoint hitpoint, t_rt *rt)
{
	int		i;
	t_vec4	col;
	t_ray	light_ray;
	double	distance;
	t_vec4	combined_light_col;
	double	received_intensity;

	// REFACTOR INTO get_hitpoint_OBJ function // also for spehere !!!
	hitpoint.pos = vec3_add(rt->camera.origin, hitpoint.ray);
	combined_light_col = VEC4_BLACK;

	i = -1;
	while (rt->lights[++i].type)
	{
		light_ray.dir = vec3_sub(hitpoint.pos, rt->lights[i].origin);
		light_ray.origin = rt->lights[i].origin;
		received_intensity = -vec3_dot(hitpoint.object->normal, vec3_normalize(light_ray.dir));
		if (received_intensity <= 0)
			continue ;
		distance = vec3_len(light_ray.dir);
		received_intensity = received_intensity / (distance * distance); // Apply Falloff
		if (received_intensity <= 0.0f) // 1 / 256 = ~0.0039
			continue ;
		if (is_obstructed(light_ray, hitpoint.object, rt)) // Check for Ligth Collision
			continue ;
		received_intensity *= rt->lights[i].intensity;
		col = (t_vec4){{rt->lights[i].color.r * received_intensity, rt->lights[i].color.g * received_intensity, rt->lights[i].color.b * received_intensity, 1.0f}};
		combined_light_col = vec4_add(combined_light_col, col);
	}

	combined_light_col.a = 1;
	combined_light_col = vec4_add(combined_light_col, rt->ambient);

	// combine Light color with base color
	combined_light_col = vec4_mul(hitpoint.object->base_color, combined_light_col);

	return (combined_light_col);
}
