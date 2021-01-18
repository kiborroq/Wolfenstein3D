/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/14 14:33:39 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/14 14:36:04 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *str1, const char *str2)
{
	unsigned char	*str1_cmp;
	unsigned char	*str2_cmp;
	int				diff;

	str1_cmp = (unsigned char *)str1;
	str2_cmp = (unsigned char *)str2;
	diff = 0;
	while (diff == 0 && (*str1_cmp || *str2_cmp))
		diff = *str1_cmp++ - *str2_cmp++;
	return (diff);
}
