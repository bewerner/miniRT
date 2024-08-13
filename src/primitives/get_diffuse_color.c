/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_diffuse_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 18:23:37 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/13 17:54:55 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_vec4	get_diffuse_color(t_hitpoint hitpoint, t_rt *rt)
{
	int		i;
	t_ray	light_ray;
	float	distance;
	t_vec4	col;
	float	intensity;

	col = VEC4_BLACK;
	i = -1;
	while (rt->lights[++i].type)
	{
		light_ray.dir = vec3_sub(vec3_add(hitpoint.pos, vec3_scale(0.001, hitpoint.normal)), rt->lights[i].origin); // offset the hitpoint to preven z-fighting
		light_ray.origin = rt->lights[i].origin;
		intensity = -vec3_dot(hitpoint.normal, vec3_normalize(light_ray.dir));
		if (intensity <= 0)
			continue ;
		distance = vec3_len(light_ray.dir);
		intensity = intensity / (distance * distance);
		if (intensity <= 0.00001 || is_obstructed(light_ray, rt)) // keep an eye on this. if light ever cuts off
			continue ;
		intensity *= rt->lights[i].intensity;
		col = vec4_add(col, vec4_scale(intensity, rt->lights[i].color));
	}
	col = vec4_add(col, rt->ambient);
	col = vec4_mul(hitpoint.object->base_color, col);
	return (col);
}

t_vec4	get_solid_color(t_hitpoint hitpoint, t_rt *rt)
{
	t_vec4			col;
	static t_vec4	vp_ambient = {{0.2, 0.2, 0.22, 1}};
	static t_vec4	vp_light = {{0.55, 0.55, 0.55, 1}};

	float dot = vec3_dot(rt->camera.viewport_light, hitpoint.normal);
	dot = fmaxf(dot, 0);
	col = vec4_scale(dot, vp_light);
	col = vec4_add(col, vp_ambient);
	return (col);
}

t_vec4	get_normal_color(t_hitpoint hitpoint, t_rt *rt)
{
	t_vec4			col;

	(void)rt;

	// real Normals
	col = (t_vec4){{hitpoint.normal.x * 0.5f + 0.5f, hitpoint.normal.y * 0.5f + 0.5f, hitpoint.normal.z * 0.5f + 0.5f, 1.0f}};

	// Achsen-Normals
	// col = (t_vec4){{fabs(hitpoint.normal.x), fabs(hitpoint.normal.y), fabs(hitpoint.normal.z), 1.0f}};
	
	return (col);
}
