/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 23:07:59 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/09 23:18:26 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	fill_image(mlx_image_t *image, t_vec4 color)
{
	uint32_t	abgr_color = vec4_to_abgr(color, false);
	ft_memset_int(image->pixels, abgr_color, image->width * image->height);
}
