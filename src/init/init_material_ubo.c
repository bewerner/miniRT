/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_material_ubo.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 17:55:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/23 23:41:04 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static size_t	get_gpu_material_count(t_material *material)
{
	size_t	count;

	count = 0;
	while (material)
	{
		count++;
		material = material->next;
	}
	return (count);
}

static void	init_ubo_materials(t_gpu_material *buffer, t_material *material)
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
		gpu_mat.normal_map_id = material->normal_map_id;
		gpu_mat.normal_strength = material->normal_strength;
		gpu_mat.metallic_texture_id = material->metallic_texture_id;
		gpu_mat.roughness_texture_id = material->roughness_texture_id;
		ft_memmove(&buffer[i], &gpu_mat, sizeof(t_gpu_material));
		i++;
		material = material->next;
	}
}

void	create_ubo_materials(t_rt *rt)
{
	size_t			size;
	t_gpu_material	*buffer;

	size = get_gpu_material_count(rt->materials) * sizeof(t_gpu_material);
	buffer = (t_gpu_material *)ft_calloc(1, size);
	if (!buffer)
		terminate("failed to allocate buffer for materials", NULL, 1, rt);
	init_ubo_materials(buffer, rt->materials);
	glGenBuffers(1, &rt->ubo_materials_id);
	glBindBuffer(GL_UNIFORM_BUFFER, rt->ubo_materials_id);
	glBufferData(GL_UNIFORM_BUFFER, size, buffer, GL_STATIC_DRAW);
	ft_free((void *)&buffer);
	glBindBufferBase(GL_UNIFORM_BUFFER, 1, rt->ubo_materials_id);
}
