/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 22:35:11 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/02 22:38:15 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	ft_timer(t_timeraction action, char *msg)
{
	static double	time;

	if (action == TIMER_START)
		time = mlx_get_time();
	else if (action == TIMER_STOP)
	{
		if (msg)
			printf("%s: %f\n", msg, mlx_get_time() - time);
		else
			printf("TIME: %f\n", mlx_get_time() - time);
	}
}
