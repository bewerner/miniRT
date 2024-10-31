/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_agx_lut_tbo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 16:21:54 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

void	create_tbo_agx_lut(char *filepath, t_rt *rt)
{
	int		width;
	int		height;
	t_vec3	*image;

	image = (t_vec3 *)stbi_loadf(filepath, &width, &height, NULL, 3);
	if (!image)
		terminate(filepath, NULL, 1, rt);
	glGenTextures(1, &rt->tbo_agx_lut_id);
	glActiveTexture(GL_TEXTURE0 + 3);
	glBindTexture(GL_TEXTURE_3D, rt->tbo_agx_lut_id);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB32F, 64, 64, 64, 0,
		GL_RGB, GL_FLOAT, image);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	stbi_image_free(image);
}
