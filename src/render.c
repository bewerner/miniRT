/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:56:20 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/02 22:47:22 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

uint32_t	get_color(int r, int g, int b, int a)
{
	return (0 | (r << 24) | (g << 16) | (b << 8) | a);
}

t_vec3	get_pixel_ray(uint32_t x, uint32_t y, t_rt *rt)
{
	t_vec3	v;

	v = vec3_add(rt->screen.pos_null, vec3_scale(x, rt->screen.x));
	v = vec3_add(v, vec3_scale(y, rt->screen.y));
	return (v);
}

void	render(t_rt *rt)
{
	uint32_t	x;
	uint32_t	y;
	t_vec3	a = rt->camera.position;
	t_vec3	b;
	// t_vec3	f = vec3_scale(rt->camera.focal_lenth, vec3_normalize(rt->camera.direction));
	// f = vec3_add(a, f);
	
	// double	t;
	double	discriminant;
	double	rr;
	// uint32_t	half_w = rt->canvas->width / 2;
	// uint32_t	half_h = rt->canvas->height / 2;

	y = 0;
	rr = rt->objects[0].radius * rt->objects[0].radius;
	t_vec3	AP = vec3_sub(a, rt->objects[0].origin);
	while (y < rt->canvas->height)
	{
		x = 0;
		while (x < rt->canvas->width)
		{
			b = get_pixel_ray(x, y, rt);
			// printf("%f, %f, %f\n", b.x, b.y, b.z);

			double	A = vec3_dot(b, b);
			double	B = 2 * vec3_dot(AP, b);
			double	C = vec3_dot(AP, AP) - rr;

			discriminant = B * B - 4 * A * C;
			// printf("%f\n", discriminant);

			if (discriminant >= 0)
				mlx_put_pixel(rt->canvas, x++, y, get_color(255, 0, 0, 255));
			else
				mlx_put_pixel(rt->canvas, x++, y, get_color(0, 0, 0, 255));
		}
		y++;
	}
}
