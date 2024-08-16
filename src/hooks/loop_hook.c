/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 19:58:12 by bwerner           #+#    #+#             */
/*   Updated: 2024/08/16 20:10:27 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

void	update_screen(t_rt *rt)
{
	float		half_w;
	float		half_h;
	t_screen	*screen;

	half_w = (rt->canvas->width - 1) / 2;
	half_h = (rt->canvas->height - 1) / 2;
	screen = &rt->screen;
	screen->x_dir = rt->camera.right;
	screen->y_dir = vec3_cross(rt->camera.direction, rt->camera.right);
	screen->origin = vec3_scale(rt->camera.focal_lenth, rt->camera.direction);
	screen->pos_null = screen->origin;
	screen->pos_null
		= vec3_sub(screen->pos_null, vec3_scale(half_w, screen->x_dir));
	screen->pos_null
		= vec3_sub(screen->pos_null, vec3_scale(half_h, screen->y_dir));
}

void	loop_hook(void *param)
{
	t_rt		*rt;
	static int	i;

	rt = (t_rt *)param;
	i++;
	handle_move_input(rt);
	move_camera(rt);
	update_screen(rt);
	if (i == 1)
		ft_timer(TIMER_START, NULL);
	if (rt->mode == MODE_SOLID)
		fill_image(rt->canvas, (t_vec4){{0.25, 0.25, 0.25, 1}});
	else if (rt->mode == MODE_NORMAL)
		fill_image(rt->canvas, (t_vec4){{0, 0, 0, 1}});
	else
		fill_image(rt->canvas, rt->ambient);
	render(rt);
	if (i == 50)
	{
		ft_timer(TIMER_STOP, NULL);
		i = 0;
	}
}
