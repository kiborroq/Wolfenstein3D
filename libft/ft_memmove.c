/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:26:31 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 13:21:14 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char *src_tmp;
	unsigned char *dest_tmp;

	src_tmp = (unsigned char *)src;
	dest_tmp = (unsigned char *)dest;
	if (dest_tmp < src_tmp)
		ft_memcpy(dest, src, n);
	if (dest_tmp > src_tmp)
		while (n-- > 0)
			dest_tmp[n] = src_tmp[n];
	return (dest);
}
