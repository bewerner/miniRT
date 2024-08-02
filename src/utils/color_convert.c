/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_convert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 22:04:33 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/03 00:19:23 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

uint32_t	combine_rgba(int r, int g, int b, int a)
{
	return (0 | (r << 24) | (g << 16) | (b << 8) | a);
}

static uint8_t	clamp(double value, uint8_t min, uint8_t max)
{
	if (value < min)
		return (min);
	else if (value > max)
		return (max);
	return (value);
}

uint32_t	vec4_to_rgba(t_vec4	col)
{
	return (0
		| ((uint8_t)(clamp(col.r * 255, 0, 255)) << 24)
		| ((uint8_t)(clamp(col.g * 255, 0, 255)) << 16)
		| ((uint8_t)(clamp(col.b * 255, 0, 255)) << 8)
		| (uint8_t)(clamp(col.a * 255, 0, 255)));
}
