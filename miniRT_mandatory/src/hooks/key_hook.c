/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hook.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:55:23 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/14 23:28:45 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_rt	*rt;

	rt = (t_rt *)param;
	if (((keydata.key == MLX_KEY_ESCAPE) || (keydata.key == MLX_KEY_Q))
		&& keydata.action == MLX_PRESS)
		mlx_close_window(rt->mlx);
}
