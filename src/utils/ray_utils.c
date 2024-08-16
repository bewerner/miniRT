/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 16:40:18 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/16 16:40:32 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/miniRT.h"

t_vec3	create_bounce_dir(t_vec3 incoming_dir, t_vec3 normal)
{
	return (vec3_sub(incoming_dir, vec3_scale((2 * vec3_dot(incoming_dir, normal)), normal)));
}
