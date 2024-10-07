/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_window_title.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/28 13:53:22 by nmihaile          #+#    #+#             */
/*   Updated: 2024/10/07 20:19:50 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

static char	*get_fps(t_rt *rt)
{
	static size_t	frame_count;
	static float	time;
	char			*fps_str;

	frame_count++;
	time += rt->delta_time;
	if (time < 0.5f)
		return (NULL);
	if (glfwGetWindowAttrib(rt->window, GLFW_FOCUSED) == GLFW_TRUE)
		printf("in focus\n");
	else
		printf("NOT in focus\n");
	fps_str = ft_itoa((1 / (time / frame_count)));
	frame_count = 0;
	time = 0;
	return (fps_str);
}

static void	add_sample_to_title(char *title, t_rt *rt)
{
	char			*sample;
	char			*max_samples;

	sample = NULL;
	max_samples = NULL;
	ft_strlcat(title, " - sample ", 1024);
	sample = ft_itoa(ft_imin(rt->sample_count, rt->max_samples));
	if (sample)
		ft_strlcat(title, sample, 1024);
	ft_strlcat(title, "/", 1024);
	max_samples = ft_itoa(rt->max_samples);
	if (max_samples)
		ft_strlcat(title, max_samples, 1024);
	free(sample);
	free(max_samples);
}

void	update_window_title(t_rt *rt)
{
	char			title[1024];
	char			*fps_str;

	title[0] = '\0';
	fps_str = get_fps(rt);
	if (!fps_str)
		return ;
	ft_strlcat(title, fps_str, 1024);
	ft_strlcat(title, " FPS - miniRT - ", 1024);
	if (!fps_str)
		return ;
	ft_strlcat(title, fps_str, 1024);
	ft_strlcat(title, " FPS - miniRT - ", 1024);
	ft_strlcat(title, rt->filename, 1024);
	if (rt->mode == MODE_PREVIEW)
		add_sample_to_title(title, rt);
	glfwSetWindowTitle(rt->window, title);
	free(fps_str);
}
