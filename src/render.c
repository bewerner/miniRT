/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:56:20 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 17:53:54 by bwerner          ###   ########.fr       */
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
			current = get_hitpoint_sphere(ray, &rt->objects[i]);
		else if (rt->objects[i].type == OBJ_PLANE)
			current = get_hitpoint_plane(ray, &rt->objects[i]);
		else if (rt->objects[i].type == OBJ_CYLINDER)
			current = get_hitpoint_cylinder(ray, &rt->objects[i]);
		if (vec3_len(current.ray) < vec3_len(closest.ray))
			closest = current;
		i++;
	}
	return (closest);
}

t_vec4	get_diffuse_color(t_hitpoint hitpoint, t_rt *rt)
{
	int		i;
	t_ray	light_ray;
	double	distance;
	t_vec4	col;
	double	intensity;

	col = VEC4_BLACK;
	i = -1;
	while (rt->lights[++i].type)
	{
		light_ray.dir = vec3_sub(hitpoint.pos, rt->lights[i].origin);
		light_ray.origin = rt->lights[i].origin;
		intensity = -vec3_dot(hitpoint.normal, vec3_normalize(light_ray.dir));
		if (intensity <= 0)
			continue ;
		distance = vec3_len(light_ray.dir);
		intensity = intensity / (distance * distance);
		if (intensity <= 0.0f || is_obstructed(light_ray, hitpoint.object, rt))
			continue ;
		intensity *= rt->lights[i].intensity;
		col = vec4_add(col, vec4_scale(intensity, rt->lights[i].color));
	}
	col = vec4_add(col, rt->ambient);
	col = vec4_mul(hitpoint.object->base_color, col);
	return (col);
}

void	trace_ray(t_ivec2 pixel, t_rt *rt)
{
	t_ray		pixel_ray;
	t_hitpoint	hitpoint;
	t_vec4		col;

	pixel_ray.origin = rt->camera.origin;
	pixel_ray.dir = get_pixel_ray(pixel.x, pixel.y, rt);
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
