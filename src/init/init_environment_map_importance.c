/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environment_map_importance.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:21:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/12/03 22:41:36 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

// weights.r = CDF
// weights.g = resolution-compensated PDF
// weights.b = ?

void	compensate_weights(t_vec3 *weights, size_t length)
{
	double scale = (double)length / (4.0f * M_PI);

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

static double get_luminance(t_vec3 color)
{
	double luminance = color.r * 0.2126 + color.g * 0.7152 + color.b * 0.0722;

	return (luminance);
}

static double get_solid_angle(double row, int width, int height)
{
	double solid_angle = sin((row / height) * M_PI) * (M_PI / height) * (2.0 * M_PI / width);

	return (solid_angle);
}

void	set_importance_weight(t_vec3 *image, t_vec3 *weights, int width, int height, size_t length)
{
	t_vec3	color;
	t_vec3	*weight;
	double	total_weight = 0;
	double	total_luminance = 0;
	double	max_luminance = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			color = image[x + y * width];
			weight = &weights[x + y * width];
			double luminance = get_luminance(color);
			double solid_angle = get_solid_angle(y + 0.5f, width, height);
			weight->r = luminance * solid_angle;
			total_weight += weight->r;

			total_luminance += luminance;
			if (luminance > max_luminance)
				max_luminance = luminance;
		}
	}
	t_rt *rt = get_rt();
    rt->env_weight_adjustment = fmin(pow(total_weight * 0.0825f, 2.6f) / 4, 1.0f);
	// printf("w %d, h %d\n", width, height);
	// printf("adjust %f\n", rt->env_weight_adjustment);
	// printf("total_weight %f\n", total_weight);
	normalize_weights(weights, total_weight, length);
}
