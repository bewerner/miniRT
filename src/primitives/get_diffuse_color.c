/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_diffuse_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 18:23:37 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/13 20:39:15 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static t_vec4	illuminate_from_point_light(t_point_light * point_light, t_hitpoint *hitpoint, t_rt *rt)
{
	t_ray	light_ray;
	float	distance;
	float	intensity;

	light_ray.dir = vec3_sub(vec3_add(hitpoint->pos, vec3_scale(0.001, hitpoint->normal)), point_light->origin); // offset the hitpoint to preven z-fighting
	light_ray.origin = point_light->origin;
	intensity = -vec3_dot(hitpoint->normal, vec3_normalize(light_ray.dir));
	if (intensity <= 0)
		return (VEC4_BLACK);
	distance = vec3_len(light_ray.dir);
	intensity = intensity / (distance * distance);
	if (intensity <= 0.000005 || is_obstructed(light_ray, rt)) // keep an eye on this. if light ever cuts off
		return (VEC4_BLACK);
	intensity *= point_light->intensity;
	return (vec4_scale(intensity, point_light->color));
}


t_vec4	get_diffuse_color(t_hitpoint hitpoint, t_rt *rt)
{
	t_light	*light;
	t_vec4	final_col;
	t_vec4	light_col;

	final_col = VEC4_BLACK;
	light = rt->lights;
	while (light)
	{
		if (light->type == LIGHT_POINT)
			light_col = illuminate_from_point_light((t_point_light *)light, &hitpoint, rt);
		final_col = vec4_add(final_col, light_col);
		light = light->next;
	}
	final_col = vec4_add(final_col, rt->ambient);
	final_col = vec4_mul(hitpoint.object->base_color, final_col);
	return (final_col);
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
