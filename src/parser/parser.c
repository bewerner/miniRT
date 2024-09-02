/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:09:09 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/02 19:01:11 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

typedef struct s_pobjs
{
	t_material	*curr_mat;
	t_object	*curr_obj;
	t_light		*curr_light;
}				t_pobjs;

static t_error	evaluate_id(t_identifier id, t_pobjs objs, t_rt *rt)
{
	if (id == ID_AMBIENT)
		return (parse_ambient(rt));
	else if (id == ID_CAMERA)
		return (parse_camera(rt));
	// else if (id == ID_MATERIAL)
	// 	return (parse_material((t_material *)objs.curr_mat, rt));
	else if (id == ID_POINT_LIGHT)
		return (parse_point_light((t_point_light *)objs.curr_light, rt));
	else if (id == ID_SPHERE)
		return (parse_sphere((t_sphere *)objs.curr_obj, rt));
	else if (id == ID_PLANE)
		return (parse_plane((t_plane *)objs.curr_obj, rt));
	else if (id == ID_CYLINDER)
		return (parse_cylinder((t_cylinder *)objs.curr_obj, rt));
	else if (id != ID_COMMENT && id != ID_MATERIAL)
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

t_error	parse_scene(size_t mat_cnt, size_t obj_cnt, size_t light_cnt, t_rt *rt)
{
	t_identifier	id;
	t_error			error;
	t_material		*curr_mat;
	t_object		*curr_obj;
	t_light			*curr_light;

	error = RT_SUCCESS;
	curr_mat = rt->materials;
	curr_obj = rt->objects;
	curr_light = rt->lights;
	// create_default_material(mat_cnt, curr_mat);
	create_materials(mat_cnt, rt);
	curr_mat = curr_mat->next;
	while (rt->line)
	{
		id = get_identifier(rt->line->content);
		error = evaluate_id(id, (t_pobjs){curr_mat, curr_obj, curr_light}, rt);
		if (error)
			return (error);
		// evaluate_material_id(id, mat_cnt, &curr_mat);
		evaluate_object_id(id, obj_cnt, &curr_obj);
		evaluate_light_id(id, light_cnt, &curr_light);
		next_lst_item(&rt->line);
	}
	verify_material_uniqueness(rt);
	return (RT_SUCCESS);
}
