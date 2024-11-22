/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environment_map_importance.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:21:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/11/22 01:10:35 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

// weights.r = cumulative distribution (CDF)
// weights.g = weight (resolution compensated PDF)
// weights.b = ?

void	compensate_weights(t_vec3 *weights, size_t length)
{
	double scale = (double)length / (2 * M_PI);

	for (size_t i = 0; i < length; i++)
		weights[i].g *= scale;
}

void	set_cumulative_distribution(t_vec3 *weights, size_t length)
{
	double	previous_weight = 0;
	double	current_weight = 0;

	for (size_t i = 0; i < length; i++)
	{
		weights[i].g = weights[i].r;
		current_weight = previous_weight + weights[i].r;
		weights[i].r = current_weight;
		previous_weight = current_weight;
	}
}

static void	normalize_weights(t_vec3 *weights, double total_weight, size_t length)
{
	for (size_t i = 0; i < length; i++)
		weights[i].r /= total_weight;
}

static double get_importance_weight(t_vec3 color, double row, int width, int height)
{
	double luminance = color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722;
	double solid_angle = sin((row / height) * M_PI) * (M_PI / height) * (2.0 * M_PI / width);

	return (luminance * solid_angle);
}

void	set_importance_weight(t_vec3 *image, t_vec3 *weights, int width, int height, size_t length)
{
	t_vec3	color;
	t_vec3	*weight;
	double	total_weight = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			color = image[x + y * width];
			weight = &weights[x + y * width];
			weight->r = get_importance_weight(color, y + 0.5f, width, height);
			total_weight += weight->r;
		}
	}
	normalize_weights(weights, total_weight, length);
}
