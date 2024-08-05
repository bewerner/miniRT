/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 16:14:02 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/05 15:00:12 by bwerner          ###   ########.fr       */
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

void	cleanup(t_rt *rt)
{
	free(rt->objects);
	free(rt->lights);
}

void	terminate(char* message, uint8_t exit_code, t_rt *rt)
{
	if (message)
		error(message);
	if (rt->mlx)
		mlx_terminate(rt->mlx);
	cleanup(rt);
	exit (exit_code);
}
