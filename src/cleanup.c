/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 16:14:02 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/21 16:07:32 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	ft_free(void **mem)
{
	if (mem && *mem)
	{
		free(*mem);
		*mem = NULL;
	}
}

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

void	error(char *message, char *msg2)
{
	if (!message)
		return ;
	ft_putendl_fd("Error", STDERR_FILENO);
	if (errno)
		perror(message);
	else if (message)
		ft_putendl_fd(message, STDERR_FILENO);
	if (msg2)
	{
		ft_putstr_fd("\033[96m-> \033[94m", STDERR_FILENO);
		ft_putstr_fd(msg2, STDERR_FILENO);
		ft_putendl_fd("\033[0m", STDERR_FILENO);
	}
	errno = 0;
}

static void	delete_shader_program(GLuint shader_program)
{
	if (shader_program)
	{
		glLinkProgram(shader_program);
		glDeleteProgram(shader_program);
	}
}

void	terminate(char *msg, char *msg2, uint8_t exit_code, t_rt *rt)
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
		error(msg, msg2);
	if (rt->window)
		glfwDestroyWindow(rt->window);
	glfwTerminate();
	if (rt->line)
		ft_lstclear(&rt->line, free);
	rt->line = NULL;
	ft_free((void *)&rt->objects);
	ft_free((void *)&rt->lights);
	ft_free((void *)&rt->materials);
	ft_free((void *)&rt->textures);
	exit (exit_code);
}
