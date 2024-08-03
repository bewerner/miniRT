/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:30:03 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/03 21:52:53 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_hitpoint	get_hitpoint_sphere(t_vec3 rayDir, t_object *object, t_rt *rt)
{
	t_hitpoint	hitpoint;
	double		discriminant;
	double		rr;
	t_vec3		AP;

	hitpoint.object = object;

	rr = object->radius * object->radius;
	AP = vec3_sub(rt->camera.origin, object->origin);

	double	A = vec3_dot(rayDir, rayDir);
	double	B = 2 * vec3_dot(AP, rayDir);
	double	C = vec3_dot(AP, AP) - rr;

	discriminant = B * B - 4 * A * C;

	if (discriminant < 0)
		return ((t_hitpoint){VEC3_INF, VEC3_INF, NULL});

	// double	t0 = (-B  sqrt(discriminant)) / (2 * A)		// DO WE NEED THIS LATER ?? 
	double	t1 = (-B - sqrt(discriminant)) / (2 * A);

	if (t1 < 0)
		return ((t_hitpoint){VEC3_INF, VEC3_INF, NULL});

	// create HitRay
	// hitpoint.pos = vec3_add(rt->camera.origin, vec3_scale(t1, rayDir));
	hitpoint.ray = vec3_scale(t1, rayDir);

	return (hitpoint);
}

t_vec4	get_diffuse_color_sphere(t_hitpoint hitpoint, t_rt *rt)
{
	t_vec4	col;
	t_vec3	normal;
	t_vec3	hitpoint_light;

	// change HitRay to HitPoint
	hitpoint.pos = vec3_add(rt->camera.origin, hitpoint.ray);

	normal = vec3_normalize(vec3_sub(hitpoint.object->origin, hitpoint.pos));
	hitpoint_light = vec3_normalize(vec3_sub(hitpoint.pos, rt->lights[0].origin));
	double	light = vec3_dot(normal, hitpoint_light);
	// col = (t_vec4){{normal.x * 0.5f + 0.5f, normal.y * 0.5f + 0.5f, normal.z * 0.5f + 0.5f, 1.0f}};

	if (light < 0)
		col = VEC4_BLACK;
	else
		col = (t_vec4){{light, 0, 0, 1.0f}};

	col = vec4_add(col, (t_vec4){{.025, .025, .1, 1}});

	return (col);
}
