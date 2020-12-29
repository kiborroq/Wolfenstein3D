/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:23:47 by kiborroq          #+#    #+#             */
/*   Updated: 2020/11/02 16:24:11 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *buf, int ch, size_t n)
{
	unsigned char *buf_tmp;
	unsigned char ch_tmp;

	buf_tmp = (unsigned char *)buf;
	ch_tmp = (unsigned char)ch;
	while (n-- > 0)
	{
		if (*buf_tmp == ch_tmp)
			return (buf_tmp);
		buf_tmp++;
	}
	return (0);
}
