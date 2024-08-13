/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:09:09 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/13 17:19:54 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static void	next_lst_item(t_list **lst)
{
	t_list			*tmp;

	tmp = *lst;
	*lst = tmp->next;
	ft_lstdelone(tmp, free);
}

static t_error	evaluate_id(t_identifier id, t_object *curr_obj, t_rt *rt)
{
	t_error	error;

	if (id == ID_AMBIENT)
		error = parse_ambient(rt);
	else if (id == ID_CAMERA)
		error = parse_camera(rt);
	else if (id == ID_LIGHT)
		error = parse_light(rt);
	else if (id == ID_SPHERE)
		error = parse_sphere((t_sphere *)curr_obj, rt);
	else if (id == ID_PLANE)
		error = parse_plane((t_plane *)curr_obj, rt);
	else if (id == ID_CYLINDER)
		error = parse_cylinder((t_cylinder *)curr_obj, rt);
	else if (id != ID_COMMENT)
		error = RT_ERROR_INVALID_IDENTIFIER;
	return (RT_SUCCESS);
}

static t_error	parse_scene(size_t obj_cnt, t_rt *rt)
{
	t_identifier	id;
	t_error			error;
	t_object		*curr_obj;

	error = RT_SUCCESS;
	curr_obj = rt->objects;
	while (rt->line)
	{
		id = get_identifier(rt->line->content);
		error = evaluate_id(id, curr_obj, rt);
		if (error)
			return (error);
		if (id >= ID_SPHERE)
		{
			obj_cnt--;
			if (obj_cnt == 0)
				curr_obj->next = NULL;
			else
				curr_obj = curr_obj->next;
		}
		next_lst_item(&rt->line);
	}
	return (RT_SUCCESS);
}

static void	load_elements(int fd, t_scene_size *scene_size, t_rt *rt)
{
	char			*line;
	t_identifier	id;

	*scene_size = (t_scene_size){0, 0, 0};
	line = prep_line(get_next_line(fd));
	while (line)
	{
		id = get_identifier(line);
		if (id == ID_INVALID)
		{
			free(line);
			close(fd);
			terminate(error_msg(RT_ERROR_INVALID_IDENTIFIER), 1, rt);
		}
		else if (id == ID_LIGHT)
			scene_size->light_cnt++;
		else if (id >= ID_SPHERE)
		{
			scene_size->obj_cnt++;
			scene_size->objs_size += obj_size(id);
		}
		if (id > ID_COMMENT)
			ft_lstadd_back(&rt->line, ft_lstnew(line));		// TODO: What aboty that?
		else
			ft_delete_line(&line);
		line = prep_line(get_next_line(fd));
	}
}

void	load_scene(char *file, t_rt *rt)
{
	int				fd;
	t_error			error;
	t_scene_size	scn_sze;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		terminate("failed to load file", 1, rt);
	load_elements(fd, &scn_sze, rt);
	close(fd);
	rt->objects = (t_object *)ft_calloc(1, scn_sze.objs_size);
	rt->lights = (t_light *)ft_calloc(scn_sze.light_cnt + 1, sizeof(t_light));
	if (rt->objects == NULL || rt->lights == NULL)
		terminate(error_msg(RT_ERROR_MALLOC), 1, rt);
	error = parse_scene(scn_sze.obj_cnt, rt);
	if (error)
		terminate(error_msg(error), 1, rt);
}
