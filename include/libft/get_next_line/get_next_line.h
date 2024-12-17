/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bwerner <bwerner@student.42heilbronn.de>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 17:18:22 by bwerner           #+#    #+#             */
/*   Updated: 2024/06/10 00:49:22 by bwerner          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 256
# endif

# include <unistd.h>
# include <stdlib.h>

typedef struct s_gnl
{
	char	*line;
	char	*jstr;
	int		nl_found;
	int		read;
	size_t	i;
	size_t	j;
}			t_gnl;

char	*get_next_line(int fd);

#endif
