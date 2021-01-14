/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uitoa.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 16:39:02 by kiborroq          #+#    #+#             */
/*   Updated: 2020/11/17 16:39:29 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_uitoa(unsigned int n)
{
	unsigned int	sep;
	int				len;
	int				i;
	char			*num;

	sep = 1;
	len = 1;
	i = 0;
	while (n / sep > 9)
	{
		len++;
		sep *= 10;
	}
	if (!(num = ft_calloc(len + 1, sizeof(char))))
		return (0);
	while (i < len)
	{
		num[i] = n / sep + '0';
		n %= sep;
		sep /= 10;
		i++;
	}
	return (num);
}
