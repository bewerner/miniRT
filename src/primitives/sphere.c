/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 21:30:03 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/05 16:57:58 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_hitpoint	get_hitpoint_sphere(t_ray ray, t_object *object)
{
	t_hitpoint	hitpoint;
	double		discriminant;
	double		rr;
	t_vec3		AP;

	hitpoint.object = object;

	rr = object->radius * object->radius;
	AP = vec3_sub(ray.origin, object->origin);

	double	A = vec3_dot(ray.dir, ray.dir);
	double	B = 2 * vec3_dot(AP, ray.dir);
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
	hitpoint.ray = vec3_scale(t1, ray.dir);

	return (hitpoint);
}

static bool	is_obstructed(t_ray ray, t_object *exclude, t_rt *rt)
{
	size_t		i;
	t_hitpoint	current;
	double		ray_len;

	current.ray = VEC3_INF;
	current.pos = VEC3_INF;
	current.object = NULL;
	ray_len = vec3_len(ray.dir);
	i = 0;
	while (rt->objects[i].type)
	{
		if (&rt->objects[i] == exclude)
		{
			i++;
			continue ;
		}
		if (rt->objects[i].type == OBJ_SPHERE )
			current = get_hitpoint_sphere(ray, &rt->objects[i]);
		// else if (rt->object[i].type == OBJ_SPHERE)
		// else if (rt->object[i].type == OBJ_SPHERE)
		if (vec3_len(current.ray) < ray_len)
			return (true);
		i++;
	}
	return (false);
}


t_vec4	get_diffuse_color_sphere(t_hitpoint hitpoint, t_rt *rt)
{
	t_vec4	col;
	t_vec3	normal;
	t_ray	light_ray;
	double	scaled_distance;
	size_t	i;

	t_vec4	combined_light_col;
	double	received_intensity;

	// change HitRay to HitPoint
	hitpoint.pos = vec3_add(rt->camera.origin, hitpoint.ray);

	normal = vec3_normalize(vec3_sub(hitpoint.object->origin, hitpoint.pos));

	combined_light_col = VEC4_BLACK;
	i = 0;
	while (rt->lights[i].type)
	{

		light_ray.dir = vec3_sub(hitpoint.pos, rt->lights[i].origin);
		light_ray.origin = rt->lights[i].origin;
		// -- top or down?
		// light_ray.dir = vec3_sub(rt->lights[i].origin, hitpoint.pos);
		// light_ray.origin = hitpoint.pos;


		received_intensity = vec3_dot(normal, vec3_normalize(light_ray.dir));
		if (received_intensity <= 0)
		{
			i++;
			continue ;
		}

		scaled_distance = vec3_len(light_ray.dir) / 10 + 1;
		// Apply Falloff
		received_intensity = received_intensity / (scaled_distance * scaled_distance); //* rt->lights[i].intensity;
		if (received_intensity <= 0.0039)
		{
			i++;
			continue ;
		}


		// Check for Ligth Collision
		if (is_obstructed(light_ray, hitpoint.object, rt))
		{
			i++;
			continue ;
		}




		col = (t_vec4){{rt->lights[i].color.r * received_intensity, rt->lights[i].color.g * received_intensity, rt->lights[i].color.b * received_intensity, 1.0f}};
		combined_light_col = vec4_add(combined_light_col, col);
		i++;
	}
	combined_light_col.a = 1;
	combined_light_col = vec4_add(combined_light_col, rt->ambient);
	// col = (t_vec4){{normal.x * 0.5f + 0.5f, normal.y * 0.5f + 0.5f, normal.z * 0.5f + 0.5f, 1.0f}};

	return (combined_light_col);


	// if (received_intensity < 0)
	// 	col = VEC4_BLACK;
	// else
	// 	col = (t_vec4){{combined_light_col.r * received_intensity, combined_light_col.g * received_intensity, light_col.b * received_intensity, 1.0f}};

	// col = vec4_add(col, rt->ambient);

	// return (col);
}
