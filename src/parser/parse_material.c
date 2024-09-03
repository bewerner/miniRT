/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 18:05:41 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/03 14:51:04 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static void	gnv_name(char *name, char **line, t_rt *rt)
{
	size_t	count;

	count = 0;
	ft_skipspace(line);
	if (**line == '\0')
		terminate("Missing value detected", 1, rt);
	if (!ft_isalpha(**line))
		terminate("material name starts not with \
an alphanumeric character", 1, rt);
	while (*line && !ft_isspace(**line)
		&& **line != ',' && count < MAX_MATERIAL_NAME - 1)
	{
		name[count++] = **line;
		(*line)++;
	}
	name[count] = '\0';
	while (*line && (ft_isspace(**line) || **line == ','))
		(*line)++;
}

static void	create_default_material(size_t mat_cnt, t_material *mat)
{
	mat->index = 0;
	if (mat_cnt == 0)
		mat->next = NULL;
	else
		mat->next = (void *)mat + sizeof(t_material);
	ft_strlcat(mat->name, "DEFAULT", MAX_MATERIAL_NAME);
	mat->color.r = 204.0f / 255.0f;
	mat->color.g = 204.0f / 255.0f;
	mat->color.b = 204.0f / 255.0f;
	mat->color.a = 1.0f;
	mat->metallic = 0.0f;
	mat->roughness = 0.5f;
	mat->ior = 1.45f;
	mat->transmission = 0.0f;
	mat->emission_strength = 0.0f;
	mat->emission_color.r = 0.0f;
	mat->emission_color.g = 0.0f;
	mat->emission_color.b = 0.0f;
	mat->emission_color.a = 0.0f;
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
	mat->color.a = 1.0f;
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
	mat->emission_color.a = 1.0f;
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

	curr_mat = rt->materials;
	create_default_material(mat_cnt++, curr_mat);
	evaluate_material_id(mat_cnt, &curr_mat);
	line = rt->line;
	while (line)
	{
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
// char			name[MAX_MATERIAL_NAME];
// t_vec4		color;
// float		metallic;
// float		roughness;
// float		ior;
// float		transmission;
// float		emission_strength;
// t_vec4		emission_color;
