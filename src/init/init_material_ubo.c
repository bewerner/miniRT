/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_material_ubo.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:55:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/02 15:38:24 by nmihaile         ###   ########.fr       */
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
		ft_memmove(&buffer[i], &gpu_mat, sizeof(t_gpu_material));
		i = i + sizeof(t_gpu_material) / sizeof(float);
		material = material->next;
	}
}

void	create_ubo_materials(t_rt *rt)
{
	size_t	size;
	GLuint	blockIndex;
	float	*buffer;

	// should we limit materials to a specific size ???
	// Needs to be checked in the parser if so…
	size = get_gpu_material_size(rt->materials);
	buffer = (float *)ft_calloc(1, size);
	if (!buffer)
		terminate("failed to allocate buffer for materials", 1, rt);
	init_ubo_materials(buffer, rt->materials);

	glGenBuffers(1, &rt->ubo_materials_id);

	glBindBuffer(GL_UNIFORM_BUFFER, rt->ubo_materials_id);
	glBufferData(GL_UNIFORM_BUFFER, size, buffer, GL_STATIC_DRAW);

	free(buffer);

	blockIndex = glGetUniformBlockIndex(rt->shader_program, "u_materials");
	if (blockIndex == GL_INVALID_INDEX)
		terminate("materials ubo not found in shader program", 1, rt);
	glUniformBlockBinding(rt->shader_program, blockIndex, 1);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, rt->ubo_materials_id);

	// size_t	size;
	// float	*buffer;
	// GLuint	texture_id;

	// size = get_gpu_material_size(rt->materials);
	// buffer = (float *)ft_calloc(1, size);
	// if (!buffer)
	// 	terminate("failed to allocate texture buffer", 1, rt);
	// init_tbo_materials(buffer, rt->materials);

	// // for(size_t i = 0; i < get_gpu_objects_size(rt->objects) / 4; i++)
	// // 	printf("%f\n", buffer[i]);

	// glGenBuffers(1, &rt->tbo_materials_id);
	// glBindBuffer(GL_TEXTURE_BUFFER, rt->tbo_materials_id);
	// glBufferData(GL_TEXTURE_BUFFER, size, buffer, GL_STATIC_DRAW);
	// glBindBuffer(GL_TEXTURE_BUFFER, 0);

	// free(buffer);

	// glGenTextures(1, &texture_id);
	// glActiveTexture(GL_TEXTURE0 + 3);
	// // rt->materials_texture_id = texture_id;
	// glBindTexture(GL_TEXTURE_BUFFER, texture_id);
	// glTexBuffer(GL_TEXTURE_BUFFER, GL_R32F, rt->tbo_materials_id);

	// GLint uniform_location = glGetUniformLocation(rt->shader_program, "materials");
	// if (uniform_location == -1)
	// 	terminate("materials not found in shader program", 1, rt);
	// glUniform1i(uniform_location, 3);
}
