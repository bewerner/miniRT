/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:09:09 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/13 11:16:52 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static t_error	parse_scene(size_t obj_count, t_rt *rt)
{
	t_identifier	id;
	t_error			error;
	t_list			*tmp;
	t_object		*curr_obj;

	error = RT_SUCCESS;
	curr_obj = rt->objects;
	while (rt->line)
	{
		id = get_identifier(rt->line->content);
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
		if (error)
			return (error);
		
		if (id >= ID_SPHERE)
		{
			if (--obj_count == 0)
				curr_obj->next = NULL;
			else
				curr_obj = curr_obj->next;
		}

		tmp = rt->line;
		rt->line = tmp->next;
		ft_lstdelone(tmp, free);
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
			scene_size->light_count++;
		else if (id >= ID_SPHERE)
		{
			scene_size->object_count++;
			scene_size->objects_size += obj_size(id);
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
	t_scene_size	scene_size;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		terminate("failed to load file", 1, rt);
	load_elements(fd, &scene_size, rt);
	close(fd);
	rt->objects = (t_object *)ft_calloc(1, scene_size.objects_size);
	rt->lights = (t_light *)ft_calloc(scene_size.light_count + 1, sizeof(t_light));
	if (rt->objects == NULL || rt->lights == NULL)
		terminate(error_msg(RT_ERROR_MALLOC), 1, rt);
	error = parse_scene(scene_size.object_count, rt);
	if (error)
		terminate(error_msg(error), 1, rt);
}
