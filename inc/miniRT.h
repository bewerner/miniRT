/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/30 15:10:39 by nmihaile          #+#    #+#             */
/*   Updated: 2024/07/30 18:48:02 by nmihaile         ###   ########.fr       */
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

# define WINDOW_WIDTH	1280 / 2
# define WINDOW_HEIGHT	720 / 2

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

// hooks.c
void	init_hooks(t_rt *rt);

// render.c
void	render(t_rt *rt);

#endif
