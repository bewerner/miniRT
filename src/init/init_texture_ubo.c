/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_texture_ubo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:49:46 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/25 19:18:46 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static size_t	get_gpu_texture_count(t_texture *texture)
{
	size_t	count;

	count = 0;
	while (texture)
	{
		count++;
		texture = texture->next;
	}
	return (count);
}

static void	init_ubo_textures(t_gpu_texture *buffer, t_texture *texture)
{
	size_t			i;
	t_gpu_texture	gpu_tex;
	static int		curr_tex_unit;

	i = 0;
	while (texture)
	{
		gpu_tex = (t_gpu_texture){0};
		gpu_tex.type = texture->type;
		if (texture->type == TEX_IMAGE)
		{
			gpu_tex.texture_unit = curr_tex_unit++;
		}
		else
			gpu_tex.texture_unit = -1;
		gpu_tex.scale = texture->scale;
		gpu_tex.col1 = texture->col1;
		gpu_tex.col2 = texture->col2;
			printf("NAME: %s TEX_UNIT: %i\n", texture->name, gpu_tex.texture_unit);
		ft_memmove(&buffer[i], &gpu_tex, sizeof(t_gpu_texture));
		i++;
		texture = texture->next;
	}
}

void	create_ubo_textures(t_rt *rt)
{
	size_t	size;
	GLuint	block_index;
	t_gpu_texture	*buffer;

	if (rt->textures == NULL)
		return ;
	size = get_gpu_texture_count(rt->textures) * sizeof(t_gpu_texture);
	buffer = (t_gpu_texture *)ft_calloc(1, size);
	if (!buffer)
		terminate("failed to allocate buffer for textures", NULL, 1, rt);
	init_ubo_textures(buffer, rt->textures);
	glGenBuffers(1, &rt->ubo_textures_id);
	glBindBuffer(GL_UNIFORM_BUFFER, rt->ubo_textures_id);
	glBufferData(GL_UNIFORM_BUFFER, size, buffer, GL_STATIC_DRAW);
	free(buffer);
	block_index
		= glGetUniformBlockIndex(rt->preview_shader_program, "u_textures");
	if (block_index == GL_INVALID_INDEX)
		terminate("textures ubo not found in shader program", NULL, 1, rt);
	glUniformBlockBinding(rt->preview_shader_program, block_index, 2);
	glBindBufferBase(GL_UNIFORM_BUFFER, 2, rt->ubo_textures_id);
}
