/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_convert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 22:04:33 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/10 20:14:58 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

uint32_t	combine_rgba(int r, int g, int b, int a)
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

uint32_t	vec4_to_abgr(t_vec4	col, bool dither)
{
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
