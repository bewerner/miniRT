/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_convert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 22:04:33 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/10 20:06:51 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

// uint32_t	combine_rgba(int r, int g, int b, int a)
// {
// 	return (0 | (r << 24) | (g << 16) | (b << 8) | a);
// }

uint32_t	rgba(int r, int g, int b, int a)
{
	return (0 | (r << 24) | (g << 16) | (b << 8) | a);
}

static float	clamp(float value, float min, float max)
{
	if (value < min)
		return (min);
	else if (value > max)
		return (max);
	return (value);
}

static uint8_t	rand_dither(float value)
{
	static int	rand;

	rand = (214013 * rand + 2531011);
	if ((float)((rand >> 16) & 0x7FFF) / 32768 < value - (uint8_t)value)
		value++;
	return ((uint8_t)value);
}

// static float linear_to_gamma(float linear_component)
// {
// 	if (linear_component > 0)
// 		return sqrtf(linear_component);
// 	return 0;
// }

// t_vec3	rgb_to_srgb(t_vec3 rgb)
// {
// 	t_vec3	srgb;

// 	srgb.a = rgb.a;
// 	srgb.r = rgb.r * 3.2406 + rgb.g * -1.5372 + rgb.b * -0.4986;
// 	srgb.g = rgb.r * -0.9689 + rgb.g * 1.8758 + rgb.b * 0.0415;
// 	srgb.b = rgb.r * 0.0557 + rgb.g * -0.2040 + rgb.b * 1.0570;
// 	return (srgb);
// }

void	raw_to_standard(t_vec3 *col)
{
	static const float	gamma = 1 / 2.2f;

	if (col->r > 0)
		col->r = powf(col->r, gamma);
	if (col->g > 0)
		col->g = powf(col->g, gamma);
	if (col->b > 0)
		col->b = powf(col->b, gamma);
}

uint32_t	vec4_to_abgr(t_vec4	col, bool dither)
{
	raw_to_standard(&col);
	if (dither)
	{
		return (0
			| ((uint8_t)(rand_dither(clamp(col.r, 0, 1) * 255)))
			| ((uint8_t)(rand_dither(clamp(col.g, 0, 1) * 255)) << 8)
			| ((uint8_t)(rand_dither(clamp(col.b, 0, 1) * 255)) << 16)
			| ((uint8_t)(clamp(col.a, 0, 1) * 255)) << 24);
	}
	else
	{
		return (0
			| ((uint8_t)(clamp(col.r, 0, 1) * 255))
			| ((uint8_t)(clamp(col.g, 0, 1) * 255) << 8)
			| ((uint8_t)(clamp(col.b, 0, 1) * 255) << 16)
			| ((uint8_t)(clamp(col.a, 0, 1) * 255) << 24));
	}
}

uint32_t	vec4_to_rgba(t_vec4	col, bool dither)
{
	raw_to_standard(&col);
	if (dither)
	{
		return (0
			| ((uint8_t)(rand_dither(clamp(col.r, 0, 1) * 255)) << 24)
			| ((uint8_t)(rand_dither(clamp(col.g, 0, 1) * 255)) << 16)
			| ((uint8_t)(rand_dither(clamp(col.b, 0, 1) * 255)) << 8)
			| (uint8_t)(clamp(col.a, 0, 1) * 255));
	}
	else
	{
		return (0
			| ((uint8_t)(clamp(col.r, 0, 1) * 255) << 24)
			| ((uint8_t)(clamp(col.g, 0, 1) * 255) << 16)
			| ((uint8_t)(clamp(col.b, 0, 1) * 255) << 8)
			| (uint8_t)(clamp(col.a, 0, 1) * 255));
	}
}
