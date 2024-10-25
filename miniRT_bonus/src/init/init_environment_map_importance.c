/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environment_map_importance.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:21:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/21 16:24:22 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	set_cumulative_distribution(t_vec4 *image, int width, int height)
{
	size_t	length;
	size_t	i;
	double	previous_weight;
	double	current_weight;

	length = (size_t)width * height;
	i = 0;
	previous_weight = 0;
	current_weight = 0;
	while (i < length)
	{
		current_weight = previous_weight + image[i].a;
		image[i].a = current_weight;
		previous_weight = current_weight;
		i++;
	}
}

static void	normalize_weight(
	t_vec4 *image, double total_weight, int width, int height)
{
	size_t	length;
	size_t	i;

	length = (size_t)width * height;
	i = 0;
	while (i < length)
	{
		image[i].a /= total_weight;
		i++;
	}
}

static double	get_importance_weight(
	double luminance, double row, int width, int height)
{
	double	solid_angle;
	double	importance_weight;

	solid_angle
		= sinf(row / height * M_PI) * (M_PI / height) * (2 * M_PI / width);
	importance_weight = luminance * solid_angle;
	return (importance_weight);
}

void	set_importance_weight(t_vec4 *image, int width, int height)
{
	t_vec4	*pixel;
	int		x;
	int		y;
	double	total_weight;

	total_weight = 0;
	y = 0;
	while (y < height)
	{
		x = 0;
		while (x < width)
		{
			pixel = &image[x + y * width];
			pixel->a
				= pixel->r * 0.2126 + pixel->g * 0.7152 + pixel->b * 0.0722;
			pixel->a = get_importance_weight(pixel->a, y + 1, width, height);
			total_weight += pixel->a;
			x++;
		}
		y++;
	}
	normalize_weight(image, total_weight, width, height);
}
