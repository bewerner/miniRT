/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:09:09 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 15:21:04 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static t_error	parse_scene(t_rt *rt)
{
	t_identifier	id;
	t_error			error;
	t_list			*tmp;
	static size_t	obj_count;

	error = RT_SUCCESS;
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
			error = parse_sphere(&obj_count, rt);
		else if (id == ID_PLANE)
			error = parse_plane(&obj_count, rt);
		else if (id != ID_COMMENT)
			error = RT_ERROR_INVALID_IDENTIFIER;
		if (error)
			return (error);
		tmp = rt->line;
		rt->line = tmp->next;
		ft_lstdelone(tmp, free);
	}
	return (RT_SUCCESS);
}

static t_ivec2	load_elements(int fd, t_rt *rt)
{
	char			*line;
	t_ivec2			obj_light_count;
	t_identifier	id;

	obj_light_count = (t_ivec2){0, 0};
	line = prep_line(get_next_line(fd));
	while (line)
	{
		id = get_identifier(line);
		if (id == ID_INVALID)
		{
			close(fd);
			terminate(error_msg(RT_ERROR_INVALID_IDENTIFIER), 1, rt);
		}
		else if (id == ID_LIGHT)
			obj_light_count.y++;
		else if (id >= ID_SPHERE)
			obj_light_count.x++;
		if (id >= ID_COMMENT)
			ft_lstadd_back(&rt->line, ft_lstnew(line));		// TODO: What aboty that?
		else
			free(line);
		line = prep_line(get_next_line(fd));
	}
	return (obj_light_count);
}

void	load_scene(char *file, t_rt *rt)
{
	int		fd;
	t_error	error;
	t_ivec2	obj_light_count;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		terminate("failed to load file", 1, rt);
	obj_light_count = load_elements(fd, rt);
	close(fd);
	rt->objects = (t_object *)ft_calloc(obj_light_count.x + 1, sizeof(t_object));
	rt->lights = (t_light *)ft_calloc(obj_light_count.y + 1, sizeof(t_light));
	if (rt->objects == NULL || rt->lights == NULL)
		terminate(error_msg(RT_ERROR_MALLOC), 1, rt);
	error = parse_scene(rt);
	if (error)
		terminate(error_msg(error), 1, rt);
}
