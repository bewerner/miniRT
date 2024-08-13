/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_hitpoint.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 18:27:07 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/13 17:16:40 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

bool	is_obstructed(t_ray ray, t_object *exclude, t_rt *rt)
{
	// size_t		i;
	t_hitpoint	current;
	float		ray_len;
	t_object	*object;

	current.ray = VEC3_INF;
	current.pos = VEC3_INF;
	current.object = NULL;
	ray_len = vec3_len(ray.dir);
	// i = 0;
	object = rt->objects;
	while (object)
	{
		if (object == exclude)
		{
			object = object->next;
			continue ;
		}
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
	// while (rt->objects[i].type)
	// {
	// 	if (&rt->objects[i] == exclude)
	// 	{
	// 		i++;
	// 		continue ;
	// 	}
	// 	if (rt->objects[i].type == OBJ_SPHERE )
	// 		current = get_hitpoint_sphere(ray, &rt->objects[i]);
	// 	else if (rt->objects[i].type == OBJ_PLANE)
	// 		current = get_hitpoint_plane(ray, &rt->objects[i]);
	// 	else if (rt->objects[i].type == OBJ_CYLINDER)
	// 		current = get_hitpoint_cylinder(ray, &rt->objects[i]);
	// 	if (vec3_len(current.ray) < ray_len)
	// 		return (true);
	// 	i++;
	// }
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
