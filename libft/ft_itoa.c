/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:19:50 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 12:18:59 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*convert(int sep, int len, int n)
{
	int		i;
	char	*num_str;

	num_str = (char *)malloc((len + 1) * sizeof(char));
	if (!num_str)
		return (0);
	i = 0;
	if (n < 0)
	{
		n *= -1;
		num_str[i] = '-';
		i++;
	}
	while (i < len)
	{
		num_str[i] = n / sep + 48;
		n %= sep;
		sep /= 10;
		i++;
	}
	num_str[i] = 0;
	return (num_str);
}

static char	*other_int(int n)
{
	int sep;
	int len;
	int num;

	sep = 1;
	len = 1;
	num = n;
	if (num < 0)
	{
		len++;
		num *= -1;
	}
	while (num / sep > 9)
	{
		len++;
		sep *= 10;
	}
	return (convert(sep, len, n));
}

static char	*min_int(void)
{
	char *min_int;

	min_int = ft_strdup(MIN_INT_STR);
	if (!min_int)
		return (0);
	return (min_int);
}

char		*ft_itoa(int n)
{
	char *n_str;

	if (n == MIN_INT)
		n_str = min_int();
	else
		n_str = other_int(n);
	if (!n_str)
		return (0);
	return (n_str);
}
