/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:22:53 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 13:24:04 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int ch, size_t n)
{
	unsigned char *dest_tmp;
	unsigned char *src_tmp;
	unsigned char ch_tmp;

	dest_tmp = (unsigned char *)dest;
	src_tmp = (unsigned char *)src;
	ch_tmp = (unsigned char)ch;
	while (n-- > 0)
	{
		*dest_tmp++ = *src_tmp++;
		if (*(dest_tmp - 1) == ch_tmp)
			return (dest_tmp);
	}
	return (0);
}
