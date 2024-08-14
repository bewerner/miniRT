/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:55:05 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/14 20:06:04 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	init_hooks(t_rt *rt)
{
	mlx_key_hook(rt->mlx, key_hook, rt);
	mlx_mouse_hook(rt->mlx, mouse_hook, rt);
	mlx_cursor_hook(rt->mlx, cursor_hook, rt);
	mlx_loop_hook(rt->mlx, loop_hook, rt);
}
