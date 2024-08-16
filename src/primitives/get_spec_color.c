/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_spec_color.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 14:57:12 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/16 16:44:34 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_vec4	get_spec_color(t_hitpoint hp, t_rt *rt)
{
	// t_vec4	col;
	t_vec4	col_spec;
	// t_ray	bounce_ray;
	t_light	*light;

	float	k_s = 1.1;	// specular reflection coefficient of the object
	t_vec3	L;			// unit vector pointing from the surface to the light source
	t_vec3	V;			// unit vector pointing from the surface to the viewer
	float	p = 5;		// shininess exponent of the object
	t_vec3	H;			// reflection direction
	t_point_light	*pl;

	float	intensity;

	col_spec = g_vec4_black;
	light = rt->lights;
	while (light)
	{
		if (light->type == LIGHT_POINT)
		{
			pl = (t_point_light *)light;
			L = vec3_normalize(vec3_sub(hp.pos, pl->origin));
			V = vec3_normalize(vec3_scale(-1.0f, hp.ray));
			H = vec3_normalize(vec3_add(V, L));

			intensity = k_s * powf(vec3_dot(L, H), p);
			// col = vec4_scale(intensity)
			col_spec = vec4_add(col_spec, vec4_scale(intensity, pl->color));


			// bounce_ray.origin = vec3_add(hp.pos, vec3_scale(0.001f, hp.normal));
			// bounce_ray.dir = create_bounce_ray(hp.ray, hp.normal);
		}
		// col = shoot_ray(bounce_ray, rt);
		light = light->next;
	}

		
	return (vec4_scale(1.0f, col_spec));
}
