/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_convert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 22:04:33 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 15:01:50 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

uint32_t	combine_rgba(int r, int g, int b, int a)
{
	return (0 | (r << 24) | (g << 16) | (b << 8) | a);
}

static double	clamp(double value, double min, double max)
{
	if (value < min)
		return (min);
	else if (value > max)
		return (max);
	return (value);
}





// static uint8_t	rand_dither(double value)
// {
// 	static size_t	i;
// 	static double	arr[1000];
// 	static bool		initialized;

// 	if (!initialized)
// 	{
// 		while (i < 1000)
// 		{
// 			arr[i] = (double)rand() / RAND_MAX;
// 			i++;
// 		}
// 		initialized = true;
// 		i = 0;
// 	}
// 	i++;
// 	if (arr[i % 1000] <= value - (uint8_t)value)
// 		value++;
// 	return ((uint8_t)value);
// }





// static uint8_t	rand_dither(double value)
// {
// 	if ((double)rand() / RAND_MAX <= value - (uint8_t)value)
// 		value++;
// 	return ((uint8_t)value);
// }





static uint8_t	rand_dither(double value)
{
	static int	rand;
	rand = (214013 * rand + 2531011);
	if ((double)((rand >> 16) & 0x7FFF) / 32768 <= value - (uint8_t)value)
		value++;
	return ((uint8_t)value);
}

uint32_t	vec4_to_rgba(t_vec4	col)
{
	static size_t	i;
	i++;
	if (i > 20 * 1280 * 720)
		i = 0;
	if (i < 10 * 1280 * 720)
		return (0
			| ((uint8_t)(rand_dither(clamp(col.r, 0, 1) * 255)) << 24)
			| ((uint8_t)(rand_dither(clamp(col.g, 0, 1) * 255)) << 16)
			| ((uint8_t)(rand_dither(clamp(col.b, 0, 1) * 255)) << 8)
			| (uint8_t)(clamp(col.a, 0, 1) * 255));
	return (0
		| ((uint8_t)(clamp(col.r, 0, 1) * 255) << 24)
		| ((uint8_t)(clamp(col.g, 0, 1) * 255) << 16)
		| ((uint8_t)(clamp(col.b, 0, 1) * 255) << 8)
		| (uint8_t)(clamp(col.a, 0, 1) * 255));
}