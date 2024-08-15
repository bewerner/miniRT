/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:56:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/15 20:19:59 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	mouse_hook(	mouse_key_t button,		action_t action,
					modifier_key_t modifier, void *param			)
{
	t_rt	*rt;

	rt = param;
	if (button == MLX_MOUSE_BUTTON_LEFT && action == MLX_PRESS && !modifier)
	{
		rt_select(rt);
		return ;
	}
	if (button == MLX_MOUSE_BUTTON_RIGHT && action == MLX_PRESS && !modifier)
	{
		rt->mouse_buttons_pressed++;
		// if (button == MLX_MOUSE_BUTTON_LEFT)
		// 	rt->clicked = 0.3;
		mlx_get_mouse_pos(rt->mlx,
			&rt->initial_cursor_pos.x, &rt->initial_cursor_pos.y);
		mlx_set_cursor_mode(rt->mlx, MLX_MOUSE_HIDDEN);
	}
	if (button == MLX_MOUSE_BUTTON_RIGHT && action == MLX_RELEASE && !modifier)
	{
		rt->mouse_buttons_pressed = ft_imax(0, rt->mouse_buttons_pressed - 1);
		if (!rt->mouse_buttons_pressed)
		{
			mlx_set_cursor_mode(rt->mlx, MLX_MOUSE_NORMAL);
			mlx_set_mouse_pos(rt->mlx, rt->initial_cursor_pos.x,
				rt->initial_cursor_pos.y);
		}
	}
}
