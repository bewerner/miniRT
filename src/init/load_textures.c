/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 12:43:34 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/26 16:38:44 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static void	load_texture(size_t idx, t_texture *tex, t_rt *rt)
{
	t_vec3			*image;
	t_ivec2			size;

	image = (t_vec3 *)stbi_loadf(tex->file, &size.x, &size.y, NULL, 3);
	if (image == NULL)
		terminate(tex->file, NULL, 1, rt);
	glGenTextures(1, &rt->texture_ids[idx]);
	glActiveTexture(GL_TEXTURE0 + 5 + idx);
	glBindTexture(GL_TEXTURE_2D, rt->texture_ids[idx]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, size.x, size.y, 0,
		GL_RGB, GL_FLOAT, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_image_free(image);
}

void	load_textures(t_rt *rt)
{
	t_texture		*tex;
	static size_t	idx;

	stbi_set_flip_vertically_on_load(true);
	tex = rt->textures;
	while (tex)
	{
		if (tex->type == TEX_IMAGE)
		{
			load_texture(idx, tex, rt);
			idx++;
		}
		tex = tex->next;
	}
}
