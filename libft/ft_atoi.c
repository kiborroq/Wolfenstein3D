/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:11:32 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/10 00:55:24 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_isspace(int ch)
{
	if (ch == 32 || (ch > 8 && ch < 14))
		return (1);
	return (0);
}

int			ft_atoi(const char *str)
{
	int		i;
	size_t	num;
	int		sign;

	i = 0;
	num = 0;
	sign = 1;
	while (ft_isspace(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		num = num * 10 + str[i] - '0';
		i++;
	}
	if (sign > 0 && num > MAX_LONG)
		return (-1);
	if (sign < 0 && num - 1 > MAX_LONG)
		return (0);
	return (num * sign);
}
