/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/02 16:54:36 by kiborroq          #+#    #+#             */
/*   Updated: 2020/12/17 11:37:20 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	putmin(int fd)
{
	ft_putstr_fd(MIN_INT_STR, fd);
}

static void	putany(int n, int fd)
{
	int sep;
	int mod;

	sep = 1;
	if (n < 0)
	{
		ft_putchar_fd('-', fd);
		n *= -1;
	}
	while (n / sep > 9)
		sep *= 10;
	mod = 0;
	while (sep > 0)
	{
		mod = n / sep;
		ft_putchar_fd(mod + 48, fd);
		n %= sep;
		sep /= 10;
	}
}

void		ft_putnbr_fd(int n, int fd)
{
	if (n == MIN_INT)
		putmin(fd);
	else
		putany(n, fd);
}
