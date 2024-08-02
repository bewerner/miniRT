/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_convert.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 22:04:33 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/02 22:05:01 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

uint32_t	combine_rgba(int r, int g, int b, int a)
{
	return (0 | (r << 24) | (g << 16) | (b << 8) | a);
}

uint32_t	vec4_to_rgba(t_vec4	col)
{
	return (0
		| ((uint8_t)(col.r * 255) << 24)
		| ((uint8_t)(col.g * 255) << 16)
		| ((uint8_t)(col.b * 255) << 8)
		| (uint8_t)(col.a * 255));
}
