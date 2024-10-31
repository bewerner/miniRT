/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:55:05 by bwerner           #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

void	init_hooks(t_rt *rt)
{
	glfwSetFramebufferSizeCallback(rt->window, resize_hook);
	glfwSetKeyCallback(rt->window, key_hook);
	glfwSetMouseButtonCallback(rt->window, mouse_hook);
	glfwSetCursorPosCallback(rt->window, cursor_hook);
	glfwSetScrollCallback(rt->window, scroll_hook);
	rt->move.speed = 1;
}
