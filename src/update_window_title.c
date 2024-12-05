/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_window_title.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 13:53:22 by nmihaile          #+#    #+#             */
/*   Updated: 2024/11/07 14:52:44 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/miniRT.h"

static void	init_cursor_is_settable(t_rt *rt)
{
	double	x;
	double	y;
	double	new_x;

	glfwGetCursorPos(rt->window, &x, &y);
	x = (int)x;
	y = (int)y;
	if (x > 0)
		new_x = x - 1;
	else
		new_x = x + 1;
	glfwPollEvents();
	glfwSetCursorPos(rt->window, new_x, y);
	glfwGetCursorPos(rt->window, &x, &y);
	if (x == new_x)
		rt->cursor_is_settable = 1;
	else
		rt->cursor_is_settable = 0;
}

static char	*get_fps(t_rt *rt)
{
	static size_t	frame_count;
	static float	time;
	char			*fps_str;

	frame_count++;
	time += rt->delta_time;
	if (time < 0.5f)
		return (NULL);
	fps_str = ft_itoa((1 / (time / frame_count)));
	frame_count = 0;
	time = 0;
	return (fps_str);
}

static void	add_sample_to_title(char *title, t_rt *rt)
{
	char	*sample;
	char	*max_samples;

	sample = NULL;
	max_samples = NULL;
	ft_strlcat(title, " | Sample ", 1024);
	sample = ft_itoa(ft_imin(rt->sample_count, rt->max_samples));
	if (sample)
		ft_strlcat(title, sample, 1024);
	ft_strlcat(title, "/", 1024);
	max_samples = ft_itoa(rt->max_samples);
	if (max_samples)
		ft_strlcat(title, max_samples, 1024);
	ft_free((void *)&sample);
	ft_free((void *)&max_samples);
}

static void	add_time_to_title(char *title, t_rt *rt)
{
	char	*minutes;
	char	*seconds;

	minutes = NULL;
	seconds = NULL;
	ft_strlcat(title, " | Time: ", 1024);
	seconds = ft_itoa((int)rt->render_time % 60);
	minutes = ft_itoa((int)rt->render_time / 60);
	if (minutes && ft_strlen(minutes) < 2)
		ft_strlcat(title, "0", 1024);
	if (minutes)
		ft_strlcat(title, minutes, 1024);
	ft_strlcat(title, ":", 1024);
	if (seconds && ft_strlen(seconds) < 2)
		ft_strlcat(title, "0", 1024);
	if (seconds)
		ft_strlcat(title, seconds, 1024);
	ft_free((void *)&minutes);
	ft_free((void *)&minutes);
	ft_free((void *)&seconds);
}

void	update_window_title(t_rt *rt)
{
	char		title[1024];
	char		*fps_str;
	static bool	cursor_initiated;

	if (!rt->first_update_finished)
		return ;
	else if (!cursor_initiated)
	{
		init_cursor_is_settable(rt);
		cursor_initiated = true;
	}
	title[0] = '\0';
	fps_str = get_fps(rt);
	if (!fps_str)
		return ;
	ft_strlcat(title, fps_str, 1024);
	ft_strlcat(title, " FPS | miniRT | ", 1024);
	ft_strlcat(title, rt->filename, 1024);
	if (rt->mode == MODE_RENDER)
	{
		add_time_to_title(title, rt);
		add_sample_to_title(title, rt);
	}
	glfwSetWindowTitle(rt->window, title);
	ft_free((void *)&fps_str);
}
