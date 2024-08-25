/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 22:35:11 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/25 19:17:42 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

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
