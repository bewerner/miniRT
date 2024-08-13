/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_hitpoint.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 18:27:07 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/13 17:32:02 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

bool	is_obstructed(t_ray ray, t_rt *rt)
{
	t_hitpoint	current;
	float		ray_len;
	t_object	*object;

	current.ray = VEC3_INF;
	current.pos = VEC3_INF;
	current.object = NULL;
	ray_len = vec3_len(ray.dir);
	object = rt->objects;
	while (object)
	{
		if (object->type == OBJ_SPHERE )
			current = get_hitpoint_sphere(ray, (t_sphere *)object);
		else if (object->type == OBJ_PLANE)
			current = get_hitpoint_plane(ray, (t_plane *)object);
		else if (object->type == OBJ_CYLINDER)
			current = get_hitpoint_cylinder(ray, (t_cylinder *)object);
		if (vec3_len(current.ray) < ray_len)
			return (true);
		object = object->next;
	}
	return (false);
}

t_hitpoint	get_closest_hitpoint(t_ray ray, t_rt *rt)
{
	t_object	*object;
	t_hitpoint	closest;
	t_hitpoint	current;

	closest.ray = VEC3_INF;
	closest.pos = VEC3_INF;
	closest.object = NULL;
	object = rt->objects;
	while (object)
	{
		if (object->type == OBJ_SPHERE)
			current = get_hitpoint_sphere(ray, (t_sphere *)object);
		else if (object->type == OBJ_PLANE)
			current = get_hitpoint_plane(ray, (t_plane *)object);
		else if (object->type == OBJ_CYLINDER)
			current = get_hitpoint_cylinder(ray, (t_cylinder *)object);
		current.object = object;
		if (vec3_len(current.ray) < vec3_len(closest.ray))
			closest = current;
		object = object->next;
	}
	return (closest);
}
