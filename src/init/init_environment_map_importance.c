/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environment_map_importance.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 16:21:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/11/14 20:32:43 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

// weights.r = cumulative distribution (CDF)
// weights.g = weight (resolution compensated PDF)
// weights.b = ?

void	compensate_weights(t_vec3 *weights, int width, int height)
{
	size_t	length;
	size_t	i;
	double	scale;
	double  biggest = 0;
	double  average = 0;

	scale = (double)width * height / (2 * M_PI);
	length = (size_t)width * height;
	i = 0;
	while (i < length)
	{
		weights[i].g *= scale;
		if (weights[i].g > biggest)
			biggest = weights[i].g;
		average += weights[i].g;
		i++;
	}
	average /= length;
	printf("scale: %f\n", scale);
	printf("biggest weight: %f\n", biggest);
	printf("average weight: %f\n", average);
}

void	set_cumulative_distribution(t_vec3 *weights, int width, int height)
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
		weights[i].g = weights[i].r;
		current_weight = previous_weight + weights[i].r;
		weights[i].r = current_weight;
		previous_weight = current_weight;
		i++;
	}
}

static void	normalize_weights(t_vec3 *weights, double total_weight, int width, int height)
{
	size_t	length;
	size_t	i;

	length = (size_t)width * height;
	i = 0;
	while (i < length)
	{
		weights[i].r /= total_weight;
		i++;
	}
}

static double	get_importance_weight(double luminance, double row, int width, int height)
{
	double	solid_angle;
	double	importance_weight;
	static double	biggest;

	if (luminance > biggest)
		biggest = luminance;
	if (luminance == -1)
		printf("biggest luminance: %f\n", biggest);
	solid_angle = sinf(row / height * M_PI) * (M_PI / height) * (2 * M_PI / width);
	importance_weight = luminance * solid_angle;
	return (importance_weight);
}

void	set_importance_weight(t_vec3 *image, t_vec3 *weights, int width, int height)
{
	t_vec3	*pixel;
	t_vec3	*weight;
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
			weight = &weights[x + y * width];
			weight->r = pixel->r * 0.2126 + pixel->g * 0.7152 + pixel->b * 0.0722;
			weight->r = get_importance_weight(weight->r, y + 1, width, height);
			total_weight += weight->r;
			x++;
		}
		y++;
	}
	get_importance_weight(-1, y + 1, width, height);
	normalize_weights(weights, total_weight, width, height);
}
