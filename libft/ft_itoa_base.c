/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 16:46:17 by kiborroq          #+#    #+#             */
/*   Updated: 2020/11/17 16:46:56 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa_base(unsigned long n, char *base)
{
	unsigned long	base_len;
	unsigned long	sep;
	unsigned long	len;
	char			*num;
	unsigned long	i;

	base_len = ft_strlen(base);
	sep = 1;
	len = 1;
	while (n / sep > base_len - 1)
	{
		sep *= base_len;
		len++;
	}
	if (!(num = ft_calloc(len + 1, sizeof(char))))
		return (0);
	i = 0;
	while (i < len)
	{
		num[i] = base[n / sep];
		n %= sep;
		sep /= base_len;
		i++;
	}
	return (num);
}
