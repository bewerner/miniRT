/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_material.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 18:05:41 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/28 13:37:13 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

int	texid_from_name(char **line, t_rt *rt)
{
	char		name[MAX_NAME];
	t_texture	*tex;
	int			id;

	id = 0;
	gnv_name(&name[0], line, rt);
	tex = rt->textures;
	while (tex)
	{
		if (ft_strcmp(tex->name, name) == 0)
			return (id);
		tex = tex->next;
		id++;
	}
	terminate("material has non existant texture", rt->curr_line, 1, rt);
	return (-1);
}

static void	set_material_col_textureid(t_material *mat, char **line, t_rt *rt)
{
	if (next_is_name(*line))
	{
		mat->color_texture_id = texid_from_name(line, rt);
		mat->color = (t_vec3){{-1.0f, -1.0f, -1.0f}};
	}
	else
	{
		mat->color.r = vr(gnv(line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		mat->color.g = vr(gnv(line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		mat->color.b = vr(gnv(line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		mat->color_texture_id = -1;
	}
}

static t_error	parse_material(t_material *mat, char *line, t_rt *rt)
{
	static size_t	index;

	line += 4;
	mat->index = ++index;
	mat->next = (void *)mat + sizeof(t_material);
	gnv_name(mat->name, &line, rt);
	mat->color_texture_id = -1;
	set_material_col_textureid(mat, &line, rt);
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
	mat->bump_map_id = -1;
	parse_bump_values(mat, &line, rt);
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
		rt->curr_line = line->content;
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
