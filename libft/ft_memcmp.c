/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:24:33 by kiborroq          #+#    #+#             */
/*   Updated: 2020/11/02 16:25:10 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *buf1, const void *buf2, size_t n)
{
	size_t			i;
	int				diff;
	unsigned char	*buf1_tmp;
	unsigned char	*buf2_tmp;

	i = 0;
	diff = 0;
	buf1_tmp = (unsigned char *)buf1;
	buf2_tmp = (unsigned char *)buf2;
	while (diff == 0 && i < n)
	{
		diff = buf1_tmp[i] - buf2_tmp[i];
		i++;
	}
	return (diff);
}
