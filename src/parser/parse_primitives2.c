/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_primitives2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 15:41:08 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/10 12:33:21 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_material	*get_next_material(char *line, t_rt *rt)
{
	t_material	*material;

	ft_skipspace(&line);
	ft_terminate_after_word(line);
	if (*line == '\0')
		return (rt->materials);
	material = rt->materials;
	while (material && ft_strcmp(material->name, line) != 0)
		material = material->next;
	if (material == NULL)
		terminate("invalid material name detected", 1, rt);		// terminate if we find an ivalid material name
		// return (rt->materials);								// invalid material name defaults to default material
	return (material);
}

void	set_color_and_material(t_vec4 *col, t_material **mat, char *line, t_rt *rt)
{
	ft_skipspace(&line);
	if (ft_isalpha(*line))
	{
		*mat = get_next_material(line, rt);
		*col = (t_vec4){{(*mat)->color.r, (*mat)->color.g, (*mat)->color.b, 1.0f}};
	}
	else
	{
		col->r = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		col->g = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		col->b = vr(gnv(&line, rt) / 255.0f, (t_vec2){0.0f, 1.0f}, rt);
		col->a = 1.0f;
		*mat = get_next_material(line, rt);
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
	hb->orientation = vec3_normalize(hb->orientation);
	hb->height = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->a = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->b = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->c = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	hb->shape = vr(gnv(&line, rt), (t_vec2){-INFINITY, INFINITY}, rt);
	// if (hb->shape != -1.0f && hb->shape != 0.0f && hb->shape != 1.0f)
	// 	terminate("Hyperboloid wrong shape value, should be [-1, 0, 1]", 1, rt);
	if (fabsf(hb->a) <= EPSILON || fabsf(hb->b) == EPSILON || fabsf(hb->c) == EPSILON)
		terminate("Hyperboloid parameters a,b and c are not allowed to be 0", 1, rt);
	set_color_and_material(&hb->base_color, &hb->material, line, rt);
	return (RT_SUCCESS);
}
