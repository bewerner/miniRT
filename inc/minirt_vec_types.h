/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_vec_types.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 20:52:10 by bwerner           #+#    #+#             */
/*   Updated: 2024/09/03 13:03:15 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_VEC_TYPES_H
# define MINIRT_VEC_TYPES_H

# include <stdint.h>

typedef struct s_ivec2
{
	int32_t		x;
	int32_t		y;
}	t_ivec2;

typedef struct s_vec2
{
	float			x;
	float			y;
}	t_vec2;

typedef struct s_dvec2
{
	double			x;
	double			y;
}	t_dvec2;

typedef struct s_vec3
{
	float			x;
	float			y;
	float			z;
}	t_vec3;

typedef union s_vec4
{
	struct
	{
		float		r;
		float		g;
		float		b;
		float		a;
	};
	struct
	{
		float		x;
		float		y;
		float		z;
		float		w;
	};
}	t_vec4;

#endif
