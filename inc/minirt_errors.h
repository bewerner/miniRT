/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_errors.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:11:26 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/07 15:37:09 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_ERRORS_H
# define MINIRT_ERRORS_H

# define RT_ERROR_MSG_SUCCES "all good"
# define RT_ERROR_MSG_WRONG_FILE_FORMAT "wrong file format. (needs to be .rt)"
# define RT_ERROR_MSG_AMBIENT_LOWER_CASE "ambient identifier must be capitalized"

typedef enum e_error
{
	RT_SUCCESS,
	RT_ERROR_WRONG_FILE_FORMAT,
	RT_ERROR_AMBIENT_LOWER_CASE
}					t_error;

#endif