/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_diffuse_color.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 18:23:37 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/10 19:53:12 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static t_vec3	illuminate_from_point_light(
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

t_vec3	get_diffuse_color(t_hitpoint hitpoint, t_rt *rt)
{
	t_light	*light;
	t_vec3	final_col;
	t_vec3	light_col;

	final_col = g_vec4_black;
	light = rt->lights;
	while (light)
	{
		if (light->type == LIGHT_POINT)
			light_col = illuminate_from_point_light(
					(t_point_light *)light, &hitpoint, rt);
		final_col = vec4_add(final_col, light_col);
		light = light->next;
	}
	final_col = vec4_add(final_col, rt->ambient);
	final_col = vec4_mul(hitpoint.object->base_color, final_col);
	return (final_col);
}

t_vec3	get_solid_color(t_hitpoint hitpoint, t_rt *rt)
{
	t_vec3			col;
	static t_vec3	vp_ambient = {0.2, 0.2, 0.22, 1};
	static t_vec3	vp_light = {0.55, 0.55, 0.55, 1};
	float			dot;

	dot = vec3_dot(rt->camera.viewport_light, hitpoint.normal);
	dot = fmaxf(dot, 0);
	col = vec3_scale(dot, vp_light);
	col = vec3_add(col, vp_ambient);
	return (col);
}

t_vec3	get_normal_color(t_hitpoint hitpoint, t_rt *rt)
{
	t_vec3			col;

	(void)rt;
	col = (t_vec3){{
		hitpoint.normal.x * 0.5f + 0.5f,
		hitpoint.normal.y * 0.5f + 0.5f,
		hitpoint.normal.z * 0.5f + 0.5f, 1.0f}};
	return (col);
}

// real Normals
// Achsen-Normals
// col = (t_vec3){{fabs(hitpoint.normal.x),
//		fabs(hitpoint.normal.y), fabs(hitpoint.normal.z), 1.0f}};
