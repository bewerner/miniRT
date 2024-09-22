/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 16:14:02 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/22 13:56:09 by nmihaile         ###   ########.fr       */
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

void	error(char *message, char *add_msg)
{
	if (!message)
		return ;
	ft_putendl_fd("Error", STDERR_FILENO);
	if (errno)
		perror(message);
	else if (message)
		ft_putendl_fd(message, STDERR_FILENO);
	if (add_msg)
	{
		ft_putchar_fd('(', STDERR_FILENO);
		ft_putstr_fd(add_msg, STDERR_FILENO);
		ft_putendl_fd(")", STDERR_FILENO);
	}
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
	free(rt->textures);
	rt->textures = NULL;
}

static void	delete_shader_program(GLuint shader_program)
{
	if (shader_program)
	{
		glLinkProgram(shader_program);
		glDeleteProgram(shader_program);
	}
}

void	terminate(char *msg, char *add_msg, uint8_t exit_code, t_rt *rt)
{
	if (rt->window)
		glUseProgram(0);
	delete_shader_program(rt->preview_shader_program);
	delete_shader_program(rt->normal_shader_program);
	delete_shader_program(rt->solid_shader_program);
	delete_shader_program(rt->postprocessing_shader_program);
	if (rt->fd != -1)
		close(rt->fd);
	if (msg)
		error(msg, add_msg);
	if (rt->window)
		glfwDestroyWindow(rt->window);
	glfwTerminate();
	cleanup(rt);
	exit (exit_code);
}
