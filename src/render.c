/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:56:20 by nmihaile          #+#    #+#             */
/*   Updated: 2024/07/30 19:35:05 by nmihaile         ###   ########.fr       */
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

	y = 0;
	while (y < rt->canvas->height)
	{
		x = 0;
		while (x < rt->canvas->width)
			mlx_put_pixel(rt->canvas, x++, y, get_color(rand() % 255, 0, 0, 255));
		y++;		
	}
}
