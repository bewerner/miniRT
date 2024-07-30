/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:56:20 by nmihaile          #+#    #+#             */
/*   Updated: 2024/07/30 22:10:01 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

uint32_t	get_color(int r, int g, int b, int a)
{
	return (0 | (r << 24) | (g << 16) | (b << 8) | a);
}

void	render(t_rt *rt)
{
	uint32_t	x;
	uint32_t	y;
	t_vec3	a;
	t_vec3	b;
	// double	t;
	double	discriminant;

	y = 0;
	a = rt->camera.position;
	while (y < rt->canvas->height)
	{
		x = 0;
		while (x < rt->canvas->width)
		{
			b.x = (rt->camera.direction.x * rt->camera.focal_lenth);
			b.y = (rt->camera.direction.y * rt->camera.focal_lenth) - (rt->canvas->width / 2) + x;
			b.z = (rt->camera.direction.z * rt->camera.focal_lenth) + (rt->canvas->height / 2) - y;
			// b = rt->camera.direction;

			discriminant = pow(2 * a.x * b.x + 2 * a.y * b.y, 2) - 4 * (b.x * b.x + b.y * b.y) * (a.x * a.x + a.y * a.y - rt->objects[0].radius * rt->objects[0].radius);

			if (discriminant >= 0)
				mlx_put_pixel(rt->canvas, x++, y, get_color(255, 0, 0, 255));
			else
				mlx_put_pixel(rt->canvas, x++, y, get_color(0, 0, 0, 255));
		}
		y++;		
	}
}
