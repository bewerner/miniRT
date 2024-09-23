/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 17:27:14 by nmihaile          #+#    #+#             */
/*   Updated: 2024/09/23 11:10:22 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	verify_material_uniqueness(t_rt *rt)
{
	t_material	*curr;
	t_material	*next;
	char		add_msg[MAX_NAME];

	curr = rt->materials;
	while (curr->next)
	{
		next = curr->next;
		while (next)
		{
			if (ft_strcmp(curr->name, next->name) == 0)
			{
				ft_memset(add_msg, 0, MAX_NAME);
				ft_strlcat(add_msg, curr->name, MAX_NAME);
				terminate("duplicate material names", add_msg, 1, rt);
			}
			next = next->next;
		}
		curr = curr->next;
	}
}

void	verify_texture_uniqueness(t_rt *rt)
{
	t_texture	*curr;
	t_texture	*next;
	char		add_msg[MAX_NAME];

	curr = rt->textures;
	while (curr && curr->next)
	{
		next = curr->next;
		while (next)
		{
			if (ft_strcmp(curr->name, next->name) == 0)
			{
				ft_memset(add_msg, 0, MAX_NAME);
				ft_strlcat(add_msg, curr->name, MAX_NAME);
				terminate("duplicate texture names", add_msg, 1, rt);
			}
			next = next->next;
		}
		curr = curr->next;
	}
}
