/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scroll_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:15:35 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/31 07:18:59 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/miniRT.h"

void	scroll_hook(GLFWwindow *window, double xoffset, double yoffset)
{
	t_rt	*rt;
	float	zoom_factor;

	rt = get_rt();
	(void)window;
	(void)xoffset;
	zoom_factor = pow(1.0 + 1.0f / SCROLL_SENSITIVITY, fabs(yoffset));
	if (yoffset > 0)
		rt->move.speed = fmaxf(0, rt->move.speed * zoom_factor);
	else
		rt->move.speed = fmaxf(0, rt->move.speed * 1.0f / zoom_factor);
}
