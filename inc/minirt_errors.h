/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt_errors.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nmihaile <nmihaile@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 13:11:26 by nmihaile          #+#    #+#             */
/*   Updated: 2024/08/09 15:14:44 by nmihaile         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_ERRORS_H
# define MINIRT_ERRORS_H

# define RT_ERROR_MSG_SUCCES "all good"
# define RT_ERROR_MSG_WRONG_FILE_FORMAT "wrong file format. (needs to be .rt)"
# define RT_ERROR_MSG_INVALID_IDENTIFIER "invalid identifier"
# define RT_ERROR_MSG_AMBIENT_LOWER_CASE "ambient identifier must be capitalized"
# define RT_ERROR_MSG_CAMERA_LOWER_CASE "camera defined in lower case, only 1 camera allowd in .rt scenes. Please use capital C"
# define RT_ERROR_MSG_MALLOC "faile to allocate memory"
# define RT_ERROR_MSG_TOO_MANY_LIGHTS "too many lights, use lowercase for multiple lights, force one light with capital L"
# define RT_ERROR_MSG_TOO_MANY_CAMERAS "too many cameras, only one allowed"
# define RT_ERROR_MSG_TOO_MANY_AMBIENT "too many ambient definitions, only one allowed"
# define RT_ERROR_MSG_TOO_MANY_SPHERES "too many spheres definitions, use lowercase for many, uppercase for one allowd sphere"


typedef enum e_error
{
	RT_SUCCESS,
	RT_ERROR_WRONG_FILE_FORMAT,
	RT_ERROR_INVALID_IDENTIFIER,
	RT_ERROR_AMBIENT_LOWER_CASE,
	RT_ERROR_CAMERA_LOWER_CASE,
	RT_ERROR_MALLOC,
	RT_ERROR_TOO_MANY_LIGHTS,
	RT_ERROR_TOO_MANY_CAMERAS,
	RT_ERROR_TOO_MANY_AMBIENT,
	RT_ERROR_TOO_MANY_SPHERES
}					t_error;

#endif