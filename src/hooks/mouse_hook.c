/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hook.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:56:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/14 19:56:53 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	mouse_hook(	enum mouse_key button,		enum action action,
					enum modifier_key modifier, void *param			)
{
	t_rt	*rt;

	rt = param;
	if (action == MLX_PRESS && !modifier)
	{
		rt->mouse_buttons_pressed++;
		if (button == MLX_MOUSE_BUTTON_LEFT)
			rt->clicked = 0.3;
		mlx_get_mouse_pos(rt->mlx,
			&rt->initial_cursor_pos.x, &rt->initial_cursor_pos.y);
		mlx_set_cursor_mode(rt->mlx, MLX_MOUSE_HIDDEN);
	}
	if (action == MLX_RELEASE && !modifier)
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
