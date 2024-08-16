/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:56:20 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/16 02:53:07 by bwerner          ###   ########.fr       */
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

static bool	is_outline(t_ivec2 px, t_hitpoint hp, t_rt *rt)
{
	t_hitpoint	op;
	t_ray		ray;
	
	ray.origin = rt->camera.origin;

	if (px.x > 0)
	{
		ray.dir = get_pixel_ray(px.x - 1, px.y, rt);
		op = get_closest_hitpoint(ray, rt);
		if (op.object != hp.object)
			return (true);
	}

	ray.dir = get_pixel_ray(px.x + 1, px.y, rt);
	op = get_closest_hitpoint(ray, rt);
	if (op.object != hp.object)
		return (true);

	if (px.y > 0)
	{
		ray.dir = get_pixel_ray(px.x, px.y - 1, rt);
		op = get_closest_hitpoint(ray, rt);
		if (op.object != hp.object)
			return (true);
	}

	ray.dir = get_pixel_ray(px.x , px.y + 1, rt);
	op = get_closest_hitpoint(ray, rt);
	if (op.object != hp.object)
		return (true);

	return (false);
}

static void	rt_mlx_put_pixel(mlx_image_t *img, uint32_t x, uint32_t y, uint32_t col)
{
	if (x < img->width && y < img->height)
		mlx_put_pixel(img, x, y, col);
}

static void	put_outline_pixel(t_ivec2 px, t_rt *rt)
{
	int32_t	x, y;

	if (px.x == 0 || px.y == 0)
		return ;

	x = px.x - 1;
	y = px.y - 1;

	// printf("x %5d   y %5d\n",px.x, px.y);

	// if (px.x >= 0 && px.y >= 0)
	// {
	// }
		rt_mlx_put_pixel(rt->canvas, px.x, px.y, rgba(242, 152, 47, 255));
		rt_mlx_put_pixel(rt->canvas, px.x + 1, px.y, rgba(242, 152, 47, 255));
		rt_mlx_put_pixel(rt->canvas, px.x, px.y +1, rgba(242, 152, 47, 255));

	// if (x >= 0 && px.y >= 0)
		rt_mlx_put_pixel(rt->canvas, x, px.y, rgba(242, 152, 47, 255));
	// if (px.x >= 0 && y >= 0)
		rt_mlx_put_pixel(rt->canvas, px.x, y, rgba(242, 152, 47, 255));
	
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
	if (rt->mode == MODE_SOLID)
	{
		if (hitpoint.object->is_selected && is_outline(pixel, hitpoint, rt))
		{
			put_outline_pixel(pixel, rt);
			return ;	
		}
		col = get_solid_color(hitpoint, rt);
	}
	else if (rt->mode == MODE_NORMAL)
		col = get_normal_color(hitpoint, rt);
	else
		col = get_diffuse_color(hitpoint, rt);
	mlx_put_pixel(rt->canvas, pixel.x, pixel.y, vec4_to_rgba(col, true));
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
