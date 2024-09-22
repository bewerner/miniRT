/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 18:05:41 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/22 14:47:33 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static void	create_default_material(size_t mat_cnt, t_material *mat)
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
}

static t_error	parse_material(t_material *mat, char *line, t_rt *rt)
{
	static size_t	index;

	line += 4;
	mat->index = ++index;
	mat->next = (void *)mat + sizeof(t_material);
	gnv_name(mat->name, &line, rt);
	mat->color.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	mat->color.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	mat->color.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	mat->metallic = vr(gnv(&line, rt), (t_vec2){0.0f, 1.0f}, rt);
	mat->roughness = vr(gnv(&line, rt), (t_vec2){0.0f, 1.0f}, rt);
	mat->ior = vr(gnv(&line, rt), (t_vec2){1.0f, INFINITY}, rt);
	mat->transmission = vr(gnv(&line, rt), (t_vec2){0.0f, 1.0f}, rt);
	mat->emission_strength = vr(gnv(&line, rt), (t_vec2){0.0f, 1.0f}, rt);
	mat->emission_color.r
		= vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	mat->emission_color.g
		= vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	mat->emission_color.b
		= vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	return (RT_SUCCESS);
}

static void	evaluate_material_id(size_t mat_cnt, t_material **curr_mat)
{
	static size_t	count;

	count++;
	if (count >= mat_cnt)
		(*curr_mat)->next = NULL;
	else
		*curr_mat = (*curr_mat)->next;
}

t_error	create_materials(size_t mat_cnt, t_rt *rt)
{
	t_identifier	id;
	t_list			*line;
	t_material		*curr_mat;

	if (mat_cnt > 100)
		terminate("Too many materials, a maximum \
of 100 is supported", NULL, 1, rt);
	curr_mat = rt->materials;
	create_default_material(mat_cnt++, curr_mat);
	evaluate_material_id(mat_cnt, &curr_mat);
	line = rt->line;
	while (line)
	{
		rt->curr_line = rt->line->content;
		id = get_identifier(line->content);
		if (id == ID_MATERIAL)
		{
			parse_material(curr_mat, line->content, rt);
			evaluate_material_id(mat_cnt, &curr_mat);
		}
		line = line->next;
	}
	return (RT_SUCCESS);
}

// t_material	*next;
// char			name[MAX_NAME];
// t_vec3		color;
// float		metallic;
// float		roughness;
// float		ior;
// float		transmission;
// float		emission_strength;
// t_vec3		emission_color;
