/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/13 18:47:55 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/13 19:44:34 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static char	*error_msg_camera(size_t camera_cnt)
{
	static char	msg0[] = "No camera in found in scene.";
	static char	msg2[] = "Too many cameras in your scene, only 1 supported.";

	if (camera_cnt == 0)
		return (msg0);
	return (msg2);
}

static void	add_line(char *line, t_rt *rt)
{
	t_list	*new_item;

	new_item = ft_lstnew(line);
	if (new_item == NULL)
	{
		free(line);
		terminate(error_msg(RT_ERROR_MALLOC), NULL, 1, rt);
	}
	ft_lstadd_back(&rt->line, new_item);
}

static void	evaluate_id(t_identifier id, char *line, t_scene *scene, t_rt *rt)
{
	if (id == ID_INVALID)
	{
		free(line);
		terminate(error_msg(RT_ERROR_INVALID_IDENTIFIER), rt->curr_line, 1, rt);
	}
	else if (id == ID_CAMERA)
		scene->camera_cnt++;
	else if (id == ID_MATERIAL)
		scene->mat_cnt++;
	else if (id == ID_TEXTURE)
		scene->tex_cnt++;
	else if (id == ID_POINT_LIGHT)
	{
		scene->light_cnt++;
		scene->light_size += light_size(id);
	}
	else if (id >= ID_SPHERE)
	{
		scene->obj_cnt++;
		scene->objs_size += obj_size(id);
	}
}

static void	load_elements(t_scene *scene, t_rt *rt)
{
	char			*line;
	t_identifier	id;

	*scene = (t_scene){0, 0, 0, 0, 0, 0, 0};
	line = prep_line(get_next_line(rt->fd));
	while (line)
	{
		id = get_identifier(line);
		evaluate_id(id, line, scene, rt);
		if (id > ID_COMMENT)
			add_line(line, rt);
		else
			ft_delete_line(&line);
		line = prep_line(get_next_line(rt->fd));
	}
	close(rt->fd);
	rt->fd = -1;
	if (scene->camera_cnt != 1)
		terminate(error_msg_camera(scene->camera_cnt), NULL, 1, rt);
}

void	load_scene(char *file, t_rt *rt)
{
	t_error	error;
	t_scene	scene;

	rt->fd = open(file, O_RDONLY);
	if (rt->fd == -1)
		terminate("failed to load file", file, 1, rt);
	load_elements(&scene, rt);
	if (scene.objs_size == 0)
		terminate(error_msg(RT_ERROR_MISSING_SCENE_OBJS), NULL, 1, rt);
	rt->objects = (t_object *)ft_calloc(1, scene.objs_size);
	if (scene.light_cnt > 0)
		rt->lights = (t_light *)ft_calloc(1, scene.light_size);
	rt->materials
		= (t_material *)ft_calloc(scene.mat_cnt + 1, sizeof(t_material));
	if (scene.tex_cnt > 0)
		rt->textures
			= (t_texture *)ft_calloc(scene.tex_cnt, sizeof(t_texture));
	if (rt->objects == NULL || (scene.light_cnt > 0 && rt->lights == NULL)
		|| rt->materials == NULL)
		terminate(error_msg(RT_ERROR_MALLOC), NULL, 1, rt);
	error = parse_scene(scene, rt);
	if (error)
		terminate(error_msg(error), rt->curr_line, 1, rt);
	rt->curr_line = NULL;
}
