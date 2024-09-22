/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_texture.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 19:55:31 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/22 14:48:46 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

// size_t			index;
// t_texture		*next;
// char			name[MAX_NAME];
// t_texture_type	type;
// char			file[MAX_NAME];
// float			scale;
// t_vec3			col1;
// t_vec3			col2;

static void	set_tex_type(t_texture_type *type, char *tex_name)
{
	*type = TEX_NONE;
	if (ft_strncmp(tex_name, "image", 6) == 0)
		*type = TEX_IMAGE;
	else if (ft_strncmp(tex_name, "checker", 8) == 0)
		*type = TEX_CHECKER;
}

static t_error	parse_texture(t_texture *tex, char *line, t_rt *rt)
{
	static size_t	index;
	char			type_name[MAX_NAME];

	line += 4;
	tex->index = index++;
	tex->next = (void *)tex + sizeof(t_texture);
	gnv_name(tex->name, &line, rt);
	gnv_name(type_name, &line, rt);
	set_tex_type(&tex->type, type_name);
	if (tex->type == TEX_IMAGE)
		gnv_name(tex->file, &line, rt);
	else if (tex->type == TEX_CHECKER)
	{
		tex->scale
			= vr(gnv(&line, rt) / 255.0f, (t_vec2){-INFINITY, INFINITY}, rt);
		tex->col1.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		tex->col1.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		tex->col1.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		tex->col2.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		tex->col2.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		tex->col2.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
	}
	return (RT_SUCCESS);
}

static void	evaluate_texture_id(size_t tex_cnt, t_texture **curr_tex)
{
	static size_t	count;

	count++;
	if (count >= tex_cnt)
		(*curr_tex)->next = NULL;
	else
		*curr_tex = (*curr_tex)->next;
}

t_error	create_textures(size_t tex_cnt, t_rt *rt)
{
	t_identifier	id;
	t_list			*line;
	t_texture		*curr_tex;

	if (tex_cnt > 100)
		terminate("Too many textures, a maximum \
of 100 is supported", NULL, 1, rt);
	curr_tex = rt->textures;
	line = rt->line;
	while (line)
	{
		rt->curr_line = line->content;
		id = get_identifier(line->content);
		if (id == ID_TEXTURE)
		{
			parse_texture(curr_tex, line->content, rt);
			evaluate_texture_id(tex_cnt, &curr_tex);
		}
		line = line->next;
	}
	return (RT_SUCCESS);
}
