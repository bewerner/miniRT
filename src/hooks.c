/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 17:12:41 by nmihaile          #+#    #+#             */
/*   Updated: 2024/07/30 17:29:06 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	keyhook(mlx_key_data_t keydata, void* param)
{
	t_rt	*rt;

	rt = (t_rt*)param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(rt->mlx);
}

void	init_hooks(t_rt *rt)
{
	mlx_key_hook(rt->mlx, keyhook, rt);
}
