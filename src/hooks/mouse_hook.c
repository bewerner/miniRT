/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:56:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/07 21:08:50 by bwerner          ###   ########.fr       */
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
	double	x;
	double	y;

	rt = get_rt();
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS && !mods)
	{
		rt->mouse_buttons_pressed++;
		glfwGetCursorPos(window, &x, &y);
		rt->initial_cursor_pos = (t_dvec2){x, y};
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE && !mods)
	{
		rt->mouse_buttons_pressed = ft_imax(0, rt->mouse_buttons_pressed - 1);
		if (!rt->mouse_buttons_pressed)
		{
			glfwSetCursorPos(window, rt->initial_cursor_pos.x,
				rt->initial_cursor_pos.y);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}
