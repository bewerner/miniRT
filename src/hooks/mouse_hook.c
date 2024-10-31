/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:56:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

void	mouse_hook(GLFWwindow *window, int button, int action, int mods)
{
	t_rt	*rt;

	(void)mods;
	rt = get_rt();
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		if (rt->cursor_is_settable)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		glfwGetCursorPos(rt->window, &rt->cursor_pos.x, &rt->cursor_pos.y);
		rt->initial_cursor_pos = rt->cursor_pos;
		usleep(15000);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
