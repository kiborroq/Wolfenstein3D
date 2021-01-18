/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_numdig.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kiborroq <kiborroq@kiborroq.42.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/15 23:10:31 by kiborroq          #+#    #+#             */
/*   Updated: 2021/01/14 23:37:59 by kiborroq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_numdig(int **array, int height, int width, int dig)
{
	int i;
	int j;
	int num;

	i = 0;
	num = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			if (array[i][j] == dig)
				num++;
			j++;
		}
		i++;
	}
	return (num);
}
