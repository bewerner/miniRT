/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:10 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/28 16:37:07 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/miniRT.h"

void	validate_args(int argc, char **argv, t_rt *rt)
{
	if (argc != 2)
		terminate("usage: ./miniRt scene_file.rt", 1, rt);
	if (ft_strlen(argv[1]) <= 3
		|| !ft_strrchr(argv[1], '.')
		|| ft_strncasecmp(ft_strrchr(argv[1], '.'), ".rt\0", 4))
	{
		terminate(error_msg(RT_ERROR_WRONG_FILE_FORMAT), 1, rt);
	}
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
	rt->mode = MODE_PREVIEW;
	while (!glfwWindowShouldClose(rt->window))
		update(rt);
	terminate(NULL, 0, rt);
	return (0);
}
