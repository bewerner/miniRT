/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 16:14:02 by bwerner           #+#    #+#             */
/*   Updated: 2024/07/30 16:36:04 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	error(char *message)
{
	if (!message)
		return ;
	ft_putstr_fd("miniRT: ", STDERR_FILENO);
	if (errno)
		perror(message);
	else
		ft_putendl_fd(message, STDERR_FILENO);
	errno = 0;
}

void	cleanup(void)
{
	return ;
}

void	terminate(char* message, uint8_t exit_code, t_rt *rt)
{
	if (message)
		error(message);
	if (rt->mlx)
		mlx_terminate(rt->mlx);
	cleanup();
	exit (exit_code);
}
