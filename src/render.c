/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:56:20 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 20:42:06 by nmihaile         ###   ########.fr       */
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
	if (rt->mode == MODE_PREVIEW)
		col = get_diffuse_color(hitpoint, rt);
	else
		col = get_viewport_color(hitpoint, rt);
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
