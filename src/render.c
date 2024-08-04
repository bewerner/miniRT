/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:56:20 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/04 20:43:09 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

t_vec3	get_pixel_ray(uint32_t x, uint32_t y, t_rt *rt)
{
	t_vec3	v;

	v = vec3_add(rt->screen.pos_null, vec3_scale(x, rt->screen.x_dir));
	v = vec3_add(v, vec3_scale(y, rt->screen.y_dir));
	return (v);
}



t_hitpoint	get_closest_hitpoint(t_ray ray, t_rt *rt)
{
	size_t		i;
	t_hitpoint	closest;
	t_hitpoint	current;

	closest.ray = VEC3_INF;
	closest.pos = VEC3_INF;
	closest.object = NULL;
	i = 0;
	while (rt->objects[i].type)
	{
		if (rt->objects[i].type == OBJ_SPHERE)
			current = get_hitpoint_sphere(ray, &rt->objects[i], rt);
		// else if (rt->object[i].type == OBJ_SPHERE)
		// else if (rt->object[i].type == OBJ_SPHERE)
		if (vec3_len(current.ray) < vec3_len(closest.ray))
			closest = current;
		i++;
	}
	return (closest);
}

t_vec4	get_diffuse_color(t_hitpoint hitpoint, t_rt *rt)
{
	if (hitpoint.object->type == OBJ_SPHERE)
		return (get_diffuse_color_sphere(hitpoint, rt));
	// else if (rt->object[i].type == OBJ_SPHERE)
	// else if (rt->object[i].type == OBJ_SPHERE)
	return (VEC4_BLACK);
}

void	trace_ray(t_ivec2 pixel, t_rt *rt)
{
	t_ray		pixel_ray;
	t_hitpoint	hitpoint;
	t_vec4		col;

	pixel_ray.origin = rt->camera.origin;
	pixel_ray.dir = get_pixel_ray(pixel.x, pixel.y, rt);
	// get closest hitpoint (and object pointer)
	hitpoint = get_closest_hitpoint(pixel_ray, rt);
	if (!hitpoint.object)
		return ;
	col = get_diffuse_color(hitpoint, rt);
	mlx_put_pixel(rt->canvas, pixel.x, pixel.y, vec4_to_rgba(col));
}

void	render(t_rt *rt)
{
	t_ivec2	pixel;

	pixel.y = 0;
	while (pixel.y < (int32_t)rt->canvas->height)
	{
		pixel.x = 0;
		while (pixel.x < (int32_t)rt->canvas->width)
		{
			trace_ray(pixel, rt);
			pixel.x++;
		}
		pixel.y++;
	}
}
