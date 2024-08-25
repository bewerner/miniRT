/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:55:05 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/25 18:04:20 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	init_hooks(t_rt *rt)
{
	glfwSetFramebufferSizeCallback(rt->window, resize_hook);
	glfwSetKeyCallback(rt->window, key_hook);
	glfwSetMouseButtonCallback(rt->window, mouse_hook);
	glfwSetCursorPosCallback(rt->window, cursor_hook);

	//D mlx_key_hook(rt->mlx, key_hook, rt);
	//D mlx_mouse_hook(rt->mlx, mouse_hook, rt);
	//D mlx_cursor_hook(rt->mlx, cursor_hook, rt);
	//D mlx_resize_hook(rt->mlx, resize_hook, rt);
	// mlx_loop_hook(rt->mlx, loop_hook, rt);
}
