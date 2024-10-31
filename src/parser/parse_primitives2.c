/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_primitives2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:41:08 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

static t_material	*get_next_material(char *line, t_object *o, t_rt *rt)
{
	t_material	*material;
	char		*line_after_word;

	ft_skipspace(&line);
	line_after_word = ft_terminate_after_word(line);
	if (*line == '\0')
		return (rt->materials);
	material = rt->materials;
	while (material && ft_strcmp(material->name, line) != 0)
		material = material->next;
	if (material == NULL)
		terminate("invalid material name detected", line, 1, rt);
	line = line_after_word;
	if (line && has_next_value(line))
	{
		line[-1] = ' ';
		o->uv_scale.x = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
		o->uv_scale.y = o->uv_scale.x;
		if (!has_next_value(line))
			return (material);
		o->uv_scale.y = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	}
	return (material);
}

void	set_color_and_material_and_uv_scale(t_object *o, char *line, t_rt *rt)
{
	o->uv_scale = (t_vec2){1, 1};
	ft_skipspace(&line);
	if (ft_isalpha(*line))
	{
		o->material = get_next_material(line, o, rt);
		o->base_color = (t_vec3){{-1.0f, -1.0f, -1.0f}};
	}
	else
	{
		o->base_color.r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		o->base_color.g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		o->base_color.b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		o->material = get_next_material(line, o, rt);
	}
}

t_error	parse_hyperboloid(t_hyperboloid *hb, t_rt *rt)
{
	char			*line;

	line = (char *)rt->line->content + 2;
	hb->type = OBJ_HYPERBOLOID;
	hb->next = (void *)hb + sizeof(t_hyperboloid);
	hb->origin.x = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->origin.y = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->origin.z = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->orientation.x = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->orientation.y = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->orientation.z = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->orientation = vn(vec3_normalize(hb->orientation), rt);
	hb->height = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->a = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->b = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->c = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->shape = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	if (fabsf(hb->a) <= EPSILON
		|| fabsf(hb->b) == EPSILON
		|| fabsf(hb->c) == EPSILON)
		terminate("Hyperboloid parameters a,b,c have to be != 0", NULL, 1, rt);
	set_color_and_material_and_uv_scale((t_object *)hb, line, rt);
	return (RT_SUCCESS);
}
