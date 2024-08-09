/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/08 09:56:53 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/08 22:51:18 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

static int	get_sign(char **str)
{
	if (**str == '-')
	{
		(*str)++;
		return (-1);
	}
	else if (**str == '+')
		(*str)++;
	return (1);
}

void	ft_skipspace(char **str)
{
	while (*str && ft_isspace(**str))
		(*str)++;
}

// sign_dpoint_dplaces[1] = -1;
// sign_dpoint_dplaces[2] = 0;
// defaults are:
// ft_atod(&str, 0, int[3]{1, -1, 0})
double ft_atod(char **str, double nbr, int sign_dpoint_dplaces[3])
{
	ft_skipspace(str);
	sign_dpoint_dplaces[0] = get_sign(str);
	while (**str)
	{
		if (**str == '.')
		{
			if (sign_dpoint_dplaces[1] != -1)
				return (NAN);
			sign_dpoint_dplaces[1] = 1;
		}
		else if (ft_isdigit(**str))
		{
			nbr = nbr * 10 + (**str - '0');
			if (sign_dpoint_dplaces[1] != -1)
				sign_dpoint_dplaces[2]++;
		}
		else if (ft_isspace(**str) || **str == ',')
			break ;
		else
			return (NAN);
		(*str)++;
	}
	while (sign_dpoint_dplaces[2]-- > 0)
		nbr /= 10;
	return (nbr * sign_dpoint_dplaces[0]);
}
