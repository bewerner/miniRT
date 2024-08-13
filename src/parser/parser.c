/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 11:09:09 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/13 18:49:04 by nmihaile         ###   ########.fr       */
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

t_error	parse_scene(size_t obj_cnt, t_rt *rt)
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
