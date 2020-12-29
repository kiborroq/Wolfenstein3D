/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:38:47 by kiborroq          #+#    #+#             */
/*   Updated: 2020/11/02 16:39:08 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *str1, const char *str2, size_t n)
{
	unsigned char	*str1_cmp;
	unsigned char	*str2_cmp;
	size_t			diff;

	str1_cmp = (unsigned char *)str1;
	str2_cmp = (unsigned char *)str2;
	diff = 0;
	while (diff == 0 && n-- > 0 && (*str1_cmp || *str2_cmp))
		diff = *str1_cmp++ - *str2_cmp++;
	return (diff);
}
