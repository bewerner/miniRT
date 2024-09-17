/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 16:14:02 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/17 18:43:28 by nmihaile         ###   ########.fr       */
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
		RT_ERROR_MSG_TOO_MANY_AMBIENT,
		RT_ERROR_MSG_MISSING_SCENE_OBJS
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
	free(rt->materials);
	rt->materials = NULL;
}

void	terminate(char *msg, uint8_t exit_code, t_rt *rt)
{
	glUseProgram(0);
	glLinkProgram(rt->shader_program);
	glDeleteProgram(rt->shader_program);
	glLinkProgram(rt->normal_shader_program);
	glDeleteProgram(rt->normal_shader_program);
	glLinkProgram(rt->solid_shader_program);
	glDeleteProgram(rt->solid_shader_program);
	if (rt->fd != -1)
		close(rt->fd);
	if (msg)
		error(msg);
	if (rt->window)
		glfwDestroyWindow(rt->window);
	glfwTerminate();
	cleanup(rt);
	exit (exit_code);
}
