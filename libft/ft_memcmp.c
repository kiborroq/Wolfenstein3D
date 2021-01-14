/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:24:33 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 13:20:53 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *mem1, const void *mem2, size_t n)
{
	size_t			i;
	int				diff;
	unsigned char	*mem1_tmp;
	unsigned char	*mem2_tmp;

	i = 0;
	diff = 0;
	mem1_tmp = (unsigned char *)mem1;
	mem2_tmp = (unsigned char *)mem2;
	while (diff == 0 && i < n)
	{
		diff = mem1_tmp[i] - mem2_tmp[i];
		i++;
	}
	return (diff);
}
