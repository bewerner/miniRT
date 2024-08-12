/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:39:29 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/12 21:23:56 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

size_t	obj_size(t_identifier id)
{
	if (id == ID_SPHERE)
		return (sizeof(t_sphere));
	else if (id == ID_PLANE)
		return (sizeof(t_plane));
	else if (id == ID_CYLINDER)
		return (sizeof(t_cylinder));
	return (0);
}

void	ft_delete_line(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
	}
}
