/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:55:05 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/18 18:12:26 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	init_hooks(t_rt *rt)
{
	glfwSetFramebufferSizeCallback(rt->window, resize_hook);
	glfwSetKeyCallback(rt->window, key_hook);
	glfwSetMouseButtonCallback(rt->window, mouse_hook);
	glfwSetCursorPosCallback(rt->window, cursor_hook);
	glfwSetScrollCallback(rt->window, scroll_hook);
	rt->move.speed = 1;
}
