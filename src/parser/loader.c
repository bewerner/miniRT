/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:47:55 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/26 21:41:53 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static void	add_line(char *line, t_rt *rt)
{
	t_list	*new_item;

	new_item = ft_lstnew(line);
	if (new_item == NULL)
	{
		free(line);
		terminate(error_msg(RT_ERROR_MALLOC), 1, rt);
	}
	ft_lstadd_back(&rt->line, new_item);
}

static void	evaluate_id(t_identifier id, char *line, t_scene_size *sz, t_rt *rt)
{
	if (id == ID_INVALID)
	{
		free(line);
		terminate(error_msg(RT_ERROR_INVALID_IDENTIFIER), 1, rt);
	}
	else if (id == ID_POINT_LIGHT)
	{
		sz->light_cnt++;
		sz->light_size += light_size(id);
	}
	else if (id >= ID_SPHERE)
	{
		sz->obj_cnt++;
		sz->objs_size += obj_size(id);
	}
}

static void	load_elements(t_scene_size *scene_size, t_rt *rt)
{
	char			*line;
	t_identifier	id;

	*scene_size = (t_scene_size){0, 0, 0, 0};
	line = prep_line(get_next_line(rt->fd));
	while (line)
	{
		id = get_identifier(line);
		evaluate_id(id, line, scene_size, rt);
		if (id > ID_COMMENT)
			add_line(line, rt);
		else
			ft_delete_line(&line);
		line = prep_line(get_next_line(rt->fd));
	}
}

void	load_scene(char *file, t_rt *rt)
{
	t_error			error;
	t_scene_size	scn_sze;

	rt->fd = open(file, O_RDONLY);
	if (rt->fd == -1)
		terminate("failed to load file", 1, rt);
	load_elements(&scn_sze, rt);
	close(rt->fd);
	rt->fd = -1;
	if (scn_sze.objs_size == 0)
		terminate(error_msg(RT_ERROR_MISSING_SCENE_OBJS), 1, rt);
	rt->objects = (t_object *)ft_calloc(1, scn_sze.objs_size);
	if (scn_sze.light_cnt > 0)
		rt->lights = (t_light *)ft_calloc(1, scn_sze.light_size);
	if (rt->objects == NULL || (scn_sze.light_cnt > 0 && rt->lights == NULL))
		terminate(error_msg(RT_ERROR_MALLOC), 1, rt);
	error = parse_scene(scn_sze.obj_cnt, scn_sze.light_cnt, rt);
	if (error)
		terminate(error_msg(error), 1, rt);
	// unselect_all(rt->objects);
}