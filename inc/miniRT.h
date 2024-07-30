/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/07/30 16:36:23 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "../libft/libft.h"
# include "../MLX42/include/MLX42/MLX42.h"

# include <stdio.h>
# include <math.h>
# include <unistd.h>
# include <sys/errno.h>

# define WINDOW_WIDTH	1024
# define WINDOW_HEIGHT	1024

typedef union		u_color
{
	uint32_t		color;
	struct
	{
		uint8_t		a;
		uint8_t		b;
		uint8_t		g;
		uint8_t		r;
	};
}					t_color;

typedef struct 		s_vec2
{
	double			x;
	double			y;
}					t_vec2;

typedef struct 		s_vec3
{
	double			x;
	double			y;
	double			z;
}					t_vec3;

typedef struct		s_object
{
	
}					t_object;

typedef struct		s_rt
{
	mlx_t			*mlx;
	mlx_image_t		*canvas;
}					t_rt;

// cleanup.c
void	error(char *message);
void	terminate(char* message, uint8_t exit_code, t_rt *rt);


#endif
