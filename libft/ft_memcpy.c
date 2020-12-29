/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:25:36 by kiborroq          #+#    #+#             */
/*   Updated: 2020/11/03 15:11:13 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char *src_tmp;
	unsigned char *dest_tmp;

	src_tmp = (unsigned char *)src;
	dest_tmp = (unsigned char *)dest;
	if (dest_tmp == src_tmp)
		return (dest);
	while (n-- > 0)
		*dest_tmp++ = *src_tmp++;
	return (dest);
}
