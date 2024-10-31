/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_validations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 17:27:14 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

void	verify_material_uniqueness(t_rt *rt)
{
	t_material	*curr;
	t_material	*next;
	char		msg2[MAX_NAME];

	curr = rt->materials;
	while (curr->next)
	{
		next = curr->next;
		while (next)
		{
			if (ft_strcmp(curr->name, next->name) == 0)
			{
				ft_memset(msg2, 0, MAX_NAME);
				ft_strlcat(msg2, curr->name, MAX_NAME);
				terminate("duplicate material names", msg2, 1, rt);
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
	char		msg2[MAX_NAME];

	curr = rt->textures;
	while (curr && curr->next)
	{
		next = curr->next;
		while (next)
		{
			if (ft_strcmp(curr->name, next->name) == 0)
			{
				ft_memset(msg2, 0, MAX_NAME);
				ft_strlcat(msg2, curr->name, MAX_NAME);
				terminate("duplicate texture names", msg2, 1, rt);
			}
			next = next->next;
		}
		curr = curr->next;
	}
}
