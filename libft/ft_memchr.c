/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:23:47 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 13:27:13 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *mem, int ch, size_t n)
{
	unsigned char *mem_tmp;
	unsigned char ch_tmp;

	mem_tmp = (unsigned char *)mem;
	ch_tmp = (unsigned char)ch;
	while (n-- > 0)
	{
		if (*mem_tmp == ch_tmp)
			return (mem_tmp);
		mem_tmp++;
	}
	return (0);
}
