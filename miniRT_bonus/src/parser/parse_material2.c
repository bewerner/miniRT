/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 11:54:03 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/24 01:34:12 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	set_material_metallic_textureid(t_material *mat, char **line, t_rt *rt)
{
	if (next_is_name(*line))
	{
		mat->metallic_texture_id = texid_from_name(line, rt);
		mat->metallic = -1.0f;
	}
	else
	{
		mat->metallic = vr(gnv(line, rt), (t_vec2){0.0f, 1.0f}, rt);
		mat->metallic_texture_id = -1;
	}
}

void	set_material_roughness_textureid(t_material *mat, char **line, t_rt *rt)
{
	if (next_is_name(*line))
	{
		mat->roughness_texture_id = texid_from_name(line, rt);
		mat->roughness = -1.0f;
	}
	else
	{
		mat->roughness = vr(gnv(line, rt), (t_vec2){0.0f, 1.0f}, rt);
		mat->roughness_texture_id = -1;
	}
}

void	create_default_material(size_t mat_cnt, t_material *mat)
{
	mat->index = 0;
	if (mat_cnt == 0)
		mat->next = NULL;
	else
		mat->next = (void *)mat + sizeof(t_material);
	ft_strlcat(mat->name, "DEFAULT", MAX_NAME);
	mat->color.r = 204.0f / 255.0f;
	mat->color.g = 204.0f / 255.0f;
	mat->color.b = 204.0f / 255.0f;
	mat->metallic = 0.0f;
	mat->roughness = 0.5f;
	mat->ior = 1.45f;
	mat->transmission = 0.0f;
	mat->emission_strength = 0.0f;
	mat->emission_color.r = 0.0f;
	mat->emission_color.g = 0.0f;
	mat->emission_color.b = 0.0f;
	mat->color_texture_id = -1;
	mat->normal_map_id = -1;
	mat->normal_strength = 0.0f;
}

void	parse_normal_values(t_material *mat, char **line, t_rt *rt)
{
	if (next_is_name(*line))
	{
		mat->normal_map_id = texid_from_name(line, rt);
		mat->normal_strength = 1;
		if (has_next_value(*line))
			mat->normal_strength
				= vr(gnv(line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	}
}
