/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/09 12:27:59 by kiborroq          #+#    #+#             */
/*   Updated: 2020/11/09 14:39:05 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# define LB '\n'

/*
**Main function in get_next_line.c
*/

int		get_next_line(int fd, char **line);

/*
**Supporting functions in get_next_line_itils.c
*/

size_t	ft_strlen(const char *str);
void	*ft_memcpy(void *dest, const void *src, size_t n);
int		clear_str(char *str, int rtn);
char	*ft_strjoin(char const *s1, char const *s2);

#endif
