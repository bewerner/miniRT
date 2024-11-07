/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:10 by nmihaile          #+#    #+#             */
/*   Updated: 2024/11/07 15:56:36 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define STB_IMAGE_IMPLEMENTATION
#include "../include/miniRT.h"

void	validate_args(int argc, char **argv, t_rt *rt)
{
	if (argc != 2)
		terminate("usage: ./miniRt scene_file.rt", NULL, 1, rt);
	if (ft_strlen(argv[1]) <= 3
		|| !ft_strrchr(argv[1], '.')
		|| ft_strncasecmp(ft_strrchr(argv[1], '.'), ".rt\0", 4))
	{
		terminate(error_msg(RT_ERROR_WRONG_FILE_FORMAT), NULL, 1, rt);
	}
	rt->max_diffuse_bounces = 1;
	rt->max_glossy_bounces = 1;
	rt->max_samples = INT32_MAX - 1;
}

t_rt	*get_rt(void)
{
	static t_rt	rt;

	return (&rt);
}

int	main(int argc, char **argv)
{
	t_rt	*rt;

	rt = get_rt();
	errno = 0;
	validate_args(argc, argv, rt);
	init_mini_rt(argv, rt);
	rt->mode = MODE_RENDER;
	printf("Loading time was %.0f seconds\n", glfwGetTime());
	while (!glfwWindowShouldClose(rt->window))
		update(rt);
	terminate(NULL, NULL, 0, rt);
	return (0);
}
