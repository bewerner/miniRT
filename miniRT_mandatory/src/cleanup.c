/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 16:14:02 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/14 23:17:52 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

char	*error_msg(t_error error)
{
	const char	*msgs[11] = {
		RT_ERROR_MSG_SUCCES,
		RT_ERROR_MSG_WRONG_FILE_FORMAT,
		RT_ERROR_MSG_INVALID_IDENTIFIER,
		RT_ERROR_MSG_MALLOC,
		RT_ERROR_MSG_TOO_MANY_LIGHTS,
		RT_ERROR_MSG_TOO_MANY_CAMERAS,
		RT_ERROR_MSG_TOO_MANY_AMBIENT
	};

	return ((char *)msgs[error]);
}

void	error(char *message)
{
	if (!message)
		return ;
	ft_putendl_fd("Error", STDERR_FILENO);
	if (errno)
		perror(message);
	else if (message)
		ft_putendl_fd(message, STDERR_FILENO);
	errno = 0;
}

void	cleanup(t_rt *rt)
{
	if (rt->line)
		ft_lstclear(&rt->line, free);
	rt->line = NULL;
	free(rt->objects);
	rt->objects = NULL;
	free(rt->lights);
	rt->lights = NULL;
}

void	terminate(char *msg, uint8_t exit_code, t_rt *rt)
{
	if (rt->fd != -1)
		close(rt->fd);
	if (msg)
		error(msg);
	if (rt->mlx)
		mlx_terminate(rt->mlx);
	cleanup(rt);
	exit (exit_code);
}