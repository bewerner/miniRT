/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_texture_ubo.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 20:49:46 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/24 21:19:25 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static size_t	get_gpu_texture_count(t_texture *texture)
{
	size_t	count;

	count = 0;
	while (texture)
	{
		// count += sizeof(t_gpu_texture);
		count++;
		texture = texture->next;
	}
printf("***********************\nGPU TEXTURE COUNT : %zu\n", count);
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
			gpu_tex.texture_unit = curr_tex_unit++;
		else
			gpu_tex.texture_unit = 0.0f;
		gpu_tex.scale = texture->scale;
		gpu_tex.col1 = texture->col1;
		gpu_tex.col2 = texture->col2;
		// ft_memmove(&buffer[i], &gpu_tex, sizeof(t_gpu_texture));
		// i = i + sizeof(t_gpu_texture) / sizeof(float);
		ft_memmove(&buffer[i], &gpu_tex, sizeof(t_gpu_texture));
		i++;
		texture = texture->next;
		// printf("*************************\n");
		// printf("type %i\n", gpu_tex.type);
		// printf("unit %i\n", gpu_tex.texture_unit);
		// printf("scale %f\n", gpu_tex.scale);
		// printf("col1 %f,%f,%f\n", gpu_tex.col1.r, gpu_tex.col1.g, gpu_tex.col1.b);
		// printf("col2 %f,%f,%f\n", gpu_tex.col2.r, gpu_tex.col2.g, gpu_tex.col2.b);
	}

	for (size_t it = 0; it < i; it++)
	{
		printf("*************************\n");
		printf("type %i\n", buffer[it].type);
		printf("unit %i\n", buffer[it].texture_unit);
		printf("scale %f\n", buffer[it].scale);
		printf("col1 %f,%f,%f\n", buffer[it].col1.r, buffer[it].col1.g, buffer[it].col1.b);
		printf("col2 %f,%f,%f\n", buffer[it].col2.r, buffer[it].col2.g, buffer[it].col2.b);
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