/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_material_ubo.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:55:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/23 12:49:38 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static size_t	get_gpu_material_size(t_material *material)
{
	size_t	size;

	size = 0;
	while (material)
	{
		size += sizeof(t_gpu_material);
		material = material->next;
	}
	return (size);
}

static void	init_ubo_materials(float *buffer, t_material *material)
{
	size_t			i;
	t_gpu_material	gpu_mat;

	i = 0;
	while (material)
	{
		gpu_mat.color = material->color;
		gpu_mat.metallic = material->metallic;
		gpu_mat.roughness = material->roughness;
		gpu_mat.ior = material->ior;
		gpu_mat.transmission = material->transmission;
		gpu_mat.emission_color = material->emission_color;
		gpu_mat.emission_strength = material->emission_strength;
		gpu_mat.color_texture_id = material->color_texture_id;
		ft_memmove(&buffer[i], &gpu_mat, sizeof(t_gpu_material));
		i = i + sizeof(t_gpu_material) / sizeof(float);
		material = material->next;
	}
}

void	create_ubo_materials(t_rt *rt)
{
	size_t	size;
	GLuint	block_index;
	float	*buffer;

	size = get_gpu_material_size(rt->materials);
	buffer = (float *)ft_calloc(1, size);
	if (!buffer)
		terminate("failed to allocate buffer for materials", NULL, 1, rt);
	init_ubo_materials(buffer, rt->materials);
	glGenBuffers(1, &rt->ubo_materials_id);
	glBindBuffer(GL_UNIFORM_BUFFER, rt->ubo_materials_id);
	glBufferData(GL_UNIFORM_BUFFER, size, buffer, GL_STATIC_DRAW);
	free(buffer);
	block_index
		= glGetUniformBlockIndex(rt->preview_shader_program, "u_materials");
	if (block_index == GL_INVALID_INDEX)
		terminate("materials ubo not found in shader program", NULL, 1, rt);
	glUniformBlockBinding(rt->preview_shader_program, block_index, 1);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, rt->ubo_materials_id);
}
