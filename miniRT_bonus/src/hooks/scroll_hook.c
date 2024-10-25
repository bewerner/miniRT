/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scroll_hook.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 16:15:35 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/21 18:53:41 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

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
