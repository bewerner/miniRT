/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:10 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/14 23:38:51 by bwerner          ###   ########.fr       */
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

int	main(int argc, char **argv)
{
	t_rt	rt;

	errno = 0;
	ft_bzero(&rt, sizeof(t_rt));
	validate_args(argc, argv, &rt);
	init_mini_rt(argv, &rt);
	render(&rt);
	mlx_loop(rt.mlx);
	terminate(NULL, 0, &rt);
	return (0);
}