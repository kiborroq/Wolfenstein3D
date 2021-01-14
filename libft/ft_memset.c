/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:27:16 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 13:21:19 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *mem, int ch, size_t n)
{
	unsigned char *mem_tmp;

	mem_tmp = (unsigned char *)mem;
	while (n-- > 0)
		*mem_tmp++ = ch;
	return (mem);
}
