/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:56:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/08 00:14:18 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

// if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && !mods)
// {
// 	// rt_select(window, rt);
// 	return ;
// }

void	mouse_hook(GLFWwindow *window, int button, int action, int mods)
{
	t_rt	*rt;

	(void)mods;
	rt = get_rt();
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		rt->initial_cursor_pos = rt->cursor_pos;
		usleep(15000);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
