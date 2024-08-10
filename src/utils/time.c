/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 22:35:11 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/10 20:14:58 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	ft_timer(t_timeraction action, char *msg)
{
	static float	time;

	if (action == TIMER_START)
		time = mlx_get_time();
	else if (action == TIMER_STOP)
	{
		if (msg)
			printf("%s: %f\n", msg, mlx_get_time() - time);
		else
			printf("%sTIME:%s %f\n", LIGHTCYAN, RESET, mlx_get_time() - time);
	}
}
