/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 22:35:11 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

void	ft_timer(t_timeraction action, char *msg)
{
	static float	time;

	if (action == TIMER_START)
		time = glfwGetTime();
	else if (action == TIMER_STOP)
	{
		if (msg)
			printf("%s: %f\n", msg, glfwGetTime() - time);
		else
			printf("%sTIME:%s %f\n", LIGHTCYAN, RESET, glfwGetTime() - time);
	}
}
