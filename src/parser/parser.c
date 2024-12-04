/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:09:09 by nmihaile          #+#    #+#             */
/*   Updated: 2024/12/04 00:34:50 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

static t_error	evaluate_id(t_identifier id, t_pobjs objs, t_rt *rt)
{
	if (id == ID_AMBIENT)
		return (parse_ambient(rt));
	else if (id == ID_CAMERA)
		return (parse_camera(rt));
	else if (id == ID_RENDER_SETTINGS)
		return (parse_render_settings(rt));
	else if (id == ID_WINDOW)
		return (parse_window(rt));
	else if (id == ID_POINT_LIGHT)
		return (parse_point_light((t_point_light *)objs.curr_light, rt));
	else if (id == ID_SPHERE)
		return (parse_sphere((t_sphere *)objs.curr_obj, rt));
	else if (id == ID_PLANE)
		return (parse_plane((t_plane *)objs.curr_obj, rt));
	else if (id == ID_CYLINDER)
		return (parse_cylinder((t_cylinder *)objs.curr_obj, rt));
	else if (id == ID_HYPERBOLOID)
		return (parse_hyperboloid((t_hyperboloid *)objs.curr_obj, rt));
	else if (id != ID_COMMENT && id != ID_MATERIAL && id != ID_TEXTURE)
		return (RT_ERROR_INVALID_IDENTIFIER);
	return (RT_SUCCESS);
}

static void	evaluate_object_id(t_identifier id, size_t obj_cnt,
	t_object **curr_obj)
{
	static size_t	count;

	if (id >= ID_SPHERE)
	{
		count++;
		if (count >= obj_cnt)
			(*curr_obj)->next = NULL;
		else
			*curr_obj = (*curr_obj)->next;
	}
}

static void	evaluate_light_id(t_identifier id, size_t light_cnt,
	t_light **curr_light)
{
	static size_t	count;

	if (id == ID_POINT_LIGHT)
	{
		count++;
		if (count >= light_cnt)
			(*curr_light)->next = NULL;
		else
			*curr_light = (*curr_light)->next;
	}
}

t_error	parse_scene(t_scene scene, t_rt *rt)
{
	t_identifier	id;
	t_error			error;
	t_object		*curr_obj;
	t_light			*curr_light;

	error = RT_SUCCESS;
	curr_obj = rt->objects;
	curr_light = rt->lights;
	create_textures(scene.tex_cnt, rt);
	create_materials(scene.mat_cnt, rt);
	while (rt->line)
	{
		rt->curr_line = rt->line->content;
		id = get_identifier(rt->line->content);
		error = evaluate_id(id, (t_pobjs){curr_obj, curr_light}, rt);
		if (error)
			return (error);
		evaluate_object_id(id, scene.obj_cnt, &curr_obj);
		evaluate_light_id(id, scene.light_cnt, &curr_light);
		next_lst_item(&rt->line);
	}
	verify_material_uniqueness(rt);
	verify_texture_uniqueness(rt);
	return (RT_SUCCESS);
}
